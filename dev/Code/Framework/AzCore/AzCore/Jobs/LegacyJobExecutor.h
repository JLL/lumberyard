/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#ifndef AZCORE_JOBS_JOBEXECUTOR_H
#define AZCORE_JOBS_JOBEXECUTOR_H

#pragma once

#include <AzCore/Debug/Profiler.h>
#include <AzCore/Jobs/JobFunction.h>
#include <AzCore/std/parallel/condition_variable.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>

namespace AZ
{
    /**
    * Helper for porting legacy jobs that allows Starting and Waiting for multiple jobs asynchronously
    */
    class LegacyJobExecutor final
    {
    public:
        LegacyJobExecutor() = default;

        LegacyJobExecutor(const LegacyJobExecutor&) = delete;

        ~LegacyJobExecutor()
        {
            WaitForCompletion();
        }

        template <class Function>
        inline void StartJob(const Function& processFunction, JobContext* context = nullptr)
        {
            Job * job = aznew JobFunctionExecutorHelper<Function>(processFunction, *this, context);
            StartJobInternal(job);
        }

        // SetPostJob - This API exists to support backwards compatibility and is not a recommended pattern to be copied.
        //  Instead, create AZ::Jobs with appropriate dependencies on each other
        template <class Function>
        inline void SetPostJob(LegacyJobExecutor& postJobExecutor, const Function& processFunction, JobContext* context = nullptr)
        {
            AZStd::unique_ptr<JobExecutorHelper> postJob(aznew JobFunctionExecutorHelper<Function>(processFunction, postJobExecutor, context)); // Allocate outside the lock
            {
                LockGuard lockGuard(m_conditionLock);

                AZ_Assert(!m_postJob, "Post already set");
                AZ_Assert(m_jobCount == 0, "LegacyJobExecutor::SetPostJob() must be called before starting any jobs");
                m_postJob = std::move(postJob);
                // Note: m_jobCount is not incremented until we push the post job
            }
        }

        inline void ClearPostJob()
        {
            LockGuard lockGuard(m_conditionLock);
            m_postJob.reset();
        }

        inline void Reset()
        {
            AZ_Assert(!IsRunning(), "LegacyJobExecutor::Reset() called while jobs in flight");
        }

        inline void WaitForCompletion()
        {
            AZStd::unique_lock<decltype(m_conditionLock)> uniqueLock(m_conditionLock);

            while (m_jobCount)
            {
                AZ_PROFILE_FUNCTION_STALL(AZ::Debug::ProfileCategory::AzCore);
                m_completionCondition.wait(uniqueLock, [this] { return this->m_jobCount == 0; });
            }
        }

        // Push a logical fence that will cause WaitForCompletion to wait until PopCompletionFence is called and all jobs are complete.  Analogue to the legacy API SJobState::SetStarted()
        // Note: this does NOT fence execution of jobs in relation to each other
        inline void PushCompletionFence()
        {
            LockGuard lockGuard(m_conditionLock);
            ++m_jobCount;
        }

        // Pop a logical completion fence.  Analogue to the legacy API SJobState::SetStopped()
        inline void PopCompletionFence()
        {
            JobCompleteUpdate();
        }

        // Are there presently jobs in-flight (queued or running)?
        inline bool IsRunning()
        {
            LockGuard lockGuard(m_conditionLock);
            return m_jobCount != 0;
        }

    private:
        void JobCompleteUpdate()
        {
            AZ::u32 postCount;
            JobExecutorHelper* postJob = nullptr;
            {
                LockGuard lockGuard(m_conditionLock);

                AZ_Assert(m_jobCount, "Invalid LegacyJobExecutor::m_jobCount.");
                postCount = --m_jobCount;

                if (!postCount)
                {
                    postJob = m_postJob.release();
                    m_completionCondition.notify_all();
                }
            }

            // outside the lock...
            if (!postCount)
            {
                if (postJob)
                {
                    postJob->StartOnExecutor();
                }
            }
        }

        inline void StartJobInternal(Job * job)
        {
            {
                LockGuard lockGuard(m_conditionLock);
                ++m_jobCount;
            }

            job->Start();
        }

        class JobExecutorHelper
        {
        public:
            virtual ~JobExecutorHelper() = default;
            virtual void StartOnExecutor() = 0;
        };

        /**
        * Private Job type that notifies the owning LegacyJobExecutor of completion
        */
        template<class Function>
        class JobFunctionExecutorHelper : public JobFunction<Function>, public JobExecutorHelper
        {
        public:
            AZ_CLASS_ALLOCATOR(JobFunctionExecutorHelper, ThreadPoolAllocator, 0)

                JobFunctionExecutorHelper(typename JobFunction<Function>::FunctionCRef processFunction, LegacyJobExecutor& executor, JobContext* context)
                : JobFunction<Function>(processFunction, true /* isAutoDelete */, context)
                , m_executor(executor)
            {
            }

            void StartOnExecutor() override
            {
                m_executor.StartJobInternal(this);
            }

            void Process() override
            {
                JobFunction<Function>::m_function();

                m_executor.JobCompleteUpdate();
            }

        private:
            LegacyJobExecutor& m_executor;
        };

        template<class Function>
        friend class JobFunctionExecutorHelper; // For JobCompleteUpdate, StartJobInternal

        using Lock = AZStd::mutex;
        using LockGuard = AZStd::lock_guard<Lock>;

        AZStd::condition_variable m_completionCondition;
        Lock m_conditionLock;
        AZStd::unique_ptr<JobExecutorHelper> m_postJob;
        AZ::u32 m_jobCount = 0;
    };
}

#endif
