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
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#ifndef CRYINCLUDE_EDITOR_TRACKVIEW_TRACKVIEWCURVEEDITOR_H
#define CRYINCLUDE_EDITOR_TRACKVIEW_TRACKVIEWCURVEEDITOR_H
#pragma once


#include "TrackViewDopeSheetBase.h"
#include "TrackViewSplineCtrl.h"
#include "Controls/TimelineCtrl.h"
#include "TrackViewTimeline.h"

#include <QWidget>

namespace Ui
{
    class TrackViewCurveEditor;
}

/** CTrackViewGraph dialog.
        Placed at the same position as tracks dialog, and display spline graphs of track.
*/
class CTrackViewCurveEditor
    : public QWidget
    , public IAnimationContextListener
    , public IEditorNotifyListener
    , public ITrackViewSequenceListener
{
    friend class TrackViewCurveEditorDialog;
public:
    CTrackViewCurveEditor(QWidget* parent);
    virtual ~CTrackViewCurveEditor();

    void SetEditLock(bool bLock);

    void SetFPS(float fps);
    float GetFPS() const;
    void SetTickDisplayMode(ETVTickMode mode);

    CTrackViewSplineCtrl& GetSplineCtrl();
    void ResetSplineCtrlZoomLevel();

    void SetPlayCallback(const std::function<void()>& callback);

    // IAnimationContextListener
    virtual void OnSequenceChanged(CTrackViewSequence* pNewSequence);
    virtual void OnTimeChanged(float newTime);

protected:
    void showEvent(QShowEvent* event) override;

private:

    void OnSplineChange();
    void OnSplineCmd(UINT cmd);
    void OnSplineCmdUpdateUI();
    void OnTimelineChange();
    void OnSplineTimeMarkerChange();

    // IEditorNotifyListener
    virtual void OnEditorNotifyEvent(EEditorNotifyEvent event) override;

    //ITrackViewSequenceListener
    void OnKeysChanged(CTrackViewSequence* pSequence) override;
    void OnKeyAdded(CTrackViewKeyHandle& addedKeyHandle) override;
    void OnKeySelectionChanged(CTrackViewSequence* pSequence) override;
    void OnNodeChanged(CTrackViewNode* pNode, ENodeChangeType type) override;
    void OnNodeSelectionChanged(CTrackViewSequence* pSequence) override;
    void OnSequenceSettingsChanged(CTrackViewSequence* pSequence) override;
    //~ITrackViewSequenceListener

    void UpdateSplines();
    void UpdateTimeRange(CTrackViewSequence* pSequence);

    void AddSpline(CTrackViewTrack* pTrack);

    TrackView::CTrackViewTimelineWidget m_timelineCtrl;

    bool m_bIgnoreSelfEvents;

    bool m_bLevelClosing;

    QScopedPointer<Ui::TrackViewCurveEditor> m_ui;
};

class TrackViewCurveEditorDialog
    : public QWidget
    , public ITrackViewSequenceListener
    , public IAnimationContextListener
{
public:
    TrackViewCurveEditorDialog(QWidget* parent);
    virtual ~TrackViewCurveEditorDialog() {}

    void SetPlayCallback(const std::function<void()>& callback) { m_widget->SetPlayCallback(callback); }

    void SetEditLock(bool bLock) { m_widget->SetEditLock(bLock); }

    CTrackViewSplineCtrl& GetSplineCtrl(){ return m_widget->GetSplineCtrl(); }

    void SetFPS(float fps) { m_widget->SetFPS(fps); }
    float GetFPS() const { return m_widget->GetFPS(); }
    void SetTickDisplayMode(ETVTickMode mode) { m_widget->SetTickDisplayMode(mode); }

    virtual void OnSequenceChanged(CTrackViewSequence* pNewSequence) { m_widget->OnSequenceChanged(pNewSequence); }
    virtual void OnTimeChanged(float newTime) { m_widget->OnTimeChanged(newTime); }

    // ITrackViewSequenceListener delegation to m_widget
    void OnKeysChanged(CTrackViewSequence* pSequence) override { m_widget->OnKeysChanged(pSequence); }
    void OnKeyAdded(CTrackViewKeyHandle& addedKeyHandle) override { m_widget->OnKeyAdded(addedKeyHandle); }
    void OnKeySelectionChanged(CTrackViewSequence* pSequence) override { m_widget->OnKeySelectionChanged(pSequence); }
    void OnNodeChanged(CTrackViewNode* pNode, ENodeChangeType type) override { m_widget->OnNodeChanged(pNode, type); }
    void OnNodeSelectionChanged(CTrackViewSequence* pSequence) override { m_widget->OnNodeSelectionChanged(pSequence); }
    void OnSequenceSettingsChanged(CTrackViewSequence* pSequence) override { m_widget->OnSequenceSettingsChanged(pSequence); }
    //~ITrackViewSequenceListener

private:
    CTrackViewCurveEditor* m_widget;
};

#endif // CRYINCLUDE_EDITOR_TRACKVIEW_TRACKVIEWCURVEEDITOR_H
