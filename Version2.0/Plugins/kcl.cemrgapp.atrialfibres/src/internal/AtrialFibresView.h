/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
/*=========================================================================
 *
 * Morphological Quantification
 *
 * Cardiac Electromechanics Research Group
 * http://www.cemrgapp.com
 * orod.razeghi@kcl.ac.uk
 *
 * This software is distributed WITHOUT ANY WARRANTY or SUPPORT!
 *
=========================================================================*/

#ifndef AtrialFibresView_h
#define AtrialFibresView_h

#include <QmitkAbstractView.h>
#include <mitkSurface.h>
#include "ui_AtrialFibresViewControls.h"
#include "ui_AtrialFibresViewUIMeshing.h"
#include "ui_AtrialFibresViewUIRemesh.h"
#include "ui_AtrialFibresViewUIConvert.h"
#include "ui_AtrialScarViewUIcemrgnet.h"

#include "QmitkRenderWindow.h"
#include "mitkCommon.h"
#include "mitkDataStorage.h"
#include "mitkDataNode.h"
#include "mitkSurface.h"
#include "vtkRenderer.h"
#include "vtkTextActor.h"

#include "CemrgAtrialTools.h"

/**
  \brief AtrialFibresView

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkAbstractView
  \ingroup ${plugin_target}_internal
*/

typedef itk::Image<uint16_t,3> ImageType;
typedef itk::Image<float,3> FloatImageType;
typedef itk::Image<short,3> ShortImageType;

class AtrialFibresView : public QmitkAbstractView {

    // this is needed for all Qt objects that should have a Qt meta-object
    // (everything that derives from QObject and wants to have signal/slots)
    Q_OBJECT

public:

    static const std::string VIEW_ID;
    bool RequestProjectDirectoryFromUser();

    inline void SetAutomaticPipeline(bool isAuto){automaticPipeline=isAuto;};

    void AutomaticAnalysis();
    bool GetUserMeshingInputs();
    bool GetUserRemeshingInputs();
    bool GetUserScarProjectionInputs();
    bool GetUserConvertFormatInputs(QString inname, QString inext);
    bool LoadSurfaceChecks();

    // helper
    void SetManualModeButtons(bool b);
    void SetAutomaticModeButtons(bool b);
    void SetTagNameFromPath(QString path);
    void UserLoadSurface();
    void SetLgeAnalysis(bool b);
    QString UserIncludeLgeAnalysis(QString segPath, ImageType::Pointer seg);
    QString GetFilePath(QString type, QString extension);
    int Ask(std::string title, std::string msg);

    inline void SetManualModeButtonsOn(){SetManualModeButtons(true);};
    inline void SetManualModeButtonsOff(){SetManualModeButtons(false);};
    inline void SetAutomaticModeButtonsOn(){SetAutomaticModeButtons(true);};
    inline void SetAutomaticModeButtonsOff(){SetAutomaticModeButtons(false);};

    QString LandmarkFilesCreated(QString defaultName, QString type);

protected slots:

    /// \brief Called when the user clicks the GUI button
    void LoadDICOM();
    void ProcessIMGS();
    void ConvertNII();
    void AnalysisChoice(); // btn3
    // Automatic Pipeline
    void MeshPreprocessing();
    // Manual pipeline
    void SegmentIMGS();
    void ManualLgeRegistration();
    void IdentifyPV();
    void CreateLabelledMesh();
    void ClipperMV();
    void ClipperPV();

    void SelectLandmarks();

    // Labelled Mesh to UAC
    void MeshingOptions();
    void ConvertFormat();
    void UacCalculation();
    void UacMeshRefinement();

    void ScarProjection();
    void Reset();

protected:

    virtual void CreateQtPartControl(QWidget *parent) override;
    virtual void SetFocus() override;
    /// \brief called by QmitkFunctionality when DataManager's selection has changed
    virtual void OnSelectionChanged(
            berry::IWorkbenchPart::Pointer source, const QList<mitk::DataNode::Pointer>& nodes) override;

    Ui::AtrialFibresViewControls m_Controls;
    Ui::AtrialFibresViewUIMeshing m_UIMeshing;
    Ui::AtrialFibresViewUIRemesh m_UIRemesh;
    Ui::AtrialFibresViewUIConvert m_UIFormat;
    Ui::AtrialScarViewUIcemrgnet m_UIcemrgnet;

private:

    QString fileName, cnnPath;
    QString directory, tagName, refinedSuffix;
    std::unique_ptr<CemrgAtrialTools> atrium;
    bool automaticPipeline, analysisOnLge;
    double uiMesh_th, uiMesh_bl, uiMesh_smth, uiMesh_ds;
    double uiRemesh_max, uiRemesh_avrg, uiRemesh_min, uiRemesh_surfcorr;
    bool uiRemesh_vtk2carp, uiRemesh_extractParts;
    int uiScar_minStep, uiScar_maxStep, uiScar_projectionMethod, uiScar_thresholdMethod, uiFormat_scale;
    std::vector<double> uiScar_thresValues;
    QString uiFormat_outExt, uiFormat_outName;

    const int APPENDAGECUT   = 19;
    const int APPENDAGEUNCUT = 20;
};

#endif // AtrialFibresView_h
