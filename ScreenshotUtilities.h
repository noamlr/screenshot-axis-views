/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef SCREENSHOTUTILITIES_H
#define SCREENSHOTUTILITIES_H

//#include <qmainwindow.h>
// #include <QWidget>
#include <mitkImage.h>
#include <mitkPointSet.h>
#include <mitkStandaloneDataStorage.h>

#include "vtkRenderer.h"
#include <mitkTransferFunction.h>

#include <itkImage.h>
#include <QString>
#include <QColor>

#ifndef DOXYGEN_IGNORE

class QLineEdit;

class ScreenshotUtilities
{
  public:
    ScreenshotUtilities(int argc, char *argv[]);
    ~ScreenshotUtilities();
    virtual void Initialize();

  protected:
    void Load(int argc, char *argv[]);
    virtual void InitializeRenderWindow();

    mitk::StandaloneDataStorage::Pointer m_DataStorageOriginal;
    mitk::StandaloneDataStorage::Pointer m_DataStorageModified;

    mitk::DataNode::Pointer m_ResultNode;

    QColor m_BackgroundColor;

    QString m_TransferFunctionFile;
    QString m_OutputDir;   
    QString m_LastFile;
    QString m_PNGExtension = "PNG File (*.png)";
    QString m_JPGExtension = "JPEG File (*.jpg)";
    int m_WidthSize = 512;
    int m_HeightSize = 512;
    int m_SlicesCount = 82;
    bool m_AxisBool[6] = {0};

  private:
    void SetDefaultTransferFunction(mitk::TransferFunction::Pointer tf);
    void TakeScreenshot(vtkRenderer* renderer, unsigned int magnificationFactor, QString fileName, 
    QString filter = "", QColor backgroundColor = QColor(255,255,255));
    QString GetNewFileName(QString filePath, QString fileName, QString secondFileName);
    
};
#endif // DOXYGEN_IGNORE

#endif // STEP6_H

/**
\example ScreenshotUtilities.h
*/
