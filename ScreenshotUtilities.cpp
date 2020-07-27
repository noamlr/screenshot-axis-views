#include "ScreenshotUtilities.h"

#include "QmitkRenderWindow.h"
#include "QmitkSliceWidget.h"

#include "mitkProperties.h"
#include "mitkRenderingManager.h"
#include "mitkImageAccessByItk.h"
#include "mitkIOUtil.h"

#include "mitkTransferFunction.h"
#include "mitkTransferFunctionProperty.h"
#include "mitkTransferFunctionPropertySerializer.h"

#include "vtkRenderer.h"
#include "vtkImageWriter.h"
#include "vtkJPEGWriter.h"
#include "vtkPNGWriter.h"
#include "vtkRenderLargeImage.h"
#include <vtkCamera.h>
#include <QString>
#include <QDir>

ScreenshotUtilities::ScreenshotUtilities(int argc, char *argv[]){
  Load(argc, argv);
}

ScreenshotUtilities::~ScreenshotUtilities(){
}

void ScreenshotUtilities::Initialize(){
  //Setting the data

  mitk::StandaloneDataStorage::SetOfObjects::ConstPointer dataNodes = this->m_DataStorageOriginal->GetAll();
  
  //Reading each image and setting TransferFunction 
  for(int image_index = 0; image_index < dataNodes->Size(); image_index++){
    mitk::Image::Pointer currentMitkImage = dynamic_cast<mitk::Image *>(dataNodes->at(image_index)->GetData());
    
    this->m_ResultNode = mitk::DataNode::New();
    this->m_DataStorageModified->Add(this->m_ResultNode);
    this->m_ResultNode->SetData(currentMitkImage);
    // set some additional properties
    this->m_ResultNode->SetProperty("binary", mitk::BoolProperty::New(false));

    // Volume Rendering and Transfer function
    this->m_ResultNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));
    this->m_ResultNode->SetProperty("volumerendering.usegpu", mitk::BoolProperty::New(true));
    this->m_ResultNode->SetProperty("layer", mitk::IntProperty::New(1));

    mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
    tf->InitializeByMitkImage(currentMitkImage);

    if(this->m_TransferFunctionFile.isEmpty())
      this->SetDefaultTransferFunction(tf);
    else
      tf = mitk::TransferFunctionPropertySerializer::DeserializeTransferFunction(this->m_TransferFunctionFile.toLatin1());

    this->m_ResultNode->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(tf.GetPointer()));
    mitk::LevelWindowProperty::Pointer levWinProp = mitk::LevelWindowProperty::New();
    mitk::LevelWindow levelwindow;
    levelwindow.SetAuto(currentMitkImage);
    levWinProp->SetLevelWindow(levelwindow);
    this->m_ResultNode->SetProperty("levelwindow", levWinProp);
  }

  this->InitializeRenderWindow();

}

void ScreenshotUtilities::SetDefaultTransferFunction(mitk::TransferFunction::Pointer tf){
  // Set the color transfer function AddRGBPoint(double x, double r, double g, double b)
  // grayvalue->opacity                                                                                                                               
  tf->GetScalarOpacityFunction()->AddPoint(-1024, 0);
  tf->GetScalarOpacityFunction()->AddPoint(-907.30622617534937, 0.0031847133757961776);
  tf->GetScalarOpacityFunction()->AddPoint(-625.76944704779748, 0);
  tf->GetScalarOpacityFunction()->AddPoint(-549.05275779376507, 0.06687898089171973);
  tf->GetScalarOpacityFunction()->AddPoint(-217.28959700093719, 0.089171974522292974);
  tf->GetScalarOpacityFunction()->AddPoint(-216.70009372071229, 0);
  tf->GetScalarOpacityFunction()->AddPoint(115.90065604498591, 0);
  tf->GetScalarOpacityFunction()->AddPoint(1689.4210526315787, 0.0059880239520958556);
                                                                   
  // gradient at grayvalue->opacity                                                                                                                   
  tf->GetGradientOpacityFunction()->AddPoint(560.69500000000005, 1.000000);                                                                    

  // grayvalue->color                                                                                                                                 
  tf->GetColorTransferFunction()->AddRGBPoint(-903.32333645735707, 0.90588235294117647, 0, 0.050980392156862744);
  tf->GetColorTransferFunction()->AddRGBPoint(-857.09746954076854, 1, 0.28235294117647053, 0.10196078431372546);
  tf->GetColorTransferFunction()->AddRGBPoint(-507.02530459231491, 0.45490196078431372, 0.67843137254901964, 0.81960784313725488);
  tf->GetColorTransferFunction()->AddRGBPoint(-307.96626054358012, 1, 1, 1);
  tf->GetColorTransferFunction()->AddRGBPoint(-38.56138706654167, 1, 1, 0.74901960784313726);
}

QString ScreenshotUtilities::GetNewFileName(QString filePath, QString fileName, QString secondFileName){
  int c = 1;
  while (QFile::exists(filePath+fileName)){
    fileName = secondFileName; //QString("/3D_View1_");
    fileName += QString::number(c);
    fileName += ".png";
    c++;
  }
  return fileName;
}

void ScreenshotUtilities::InitializeRenderWindow()
{
  mitk::StandaloneDataStorage::Pointer ds = mitk::StandaloneDataStorage::New();
  QmitkRenderWindow* renderWindow = new QmitkRenderWindow();

  // Tell the renderwindow which (part of) the tree to render
  renderWindow->GetRenderer()->SetDataStorage(m_DataStorageModified);

  // Use it as a 3D view
  renderWindow->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);
  // renderWindow->resize(1024, 900);
  renderWindow->resize(512, 450);

  renderWindow->setAttribute(Qt::WA_DontShowOnScreen);
  renderWindow->show();

  vtkRenderWindow* renderWindow2 = renderWindow->GetVtkRenderWindow();
  mitk::BaseRenderer* baserenderer = mitk::BaseRenderer::GetInstance(renderWindow2);
  auto renderer = baserenderer->GetVtkRenderer();

  // Reposition the camera to include all visible actors
  renderer->ResetCamera();
  vtkCamera* vtkcam = renderer->GetActiveCamera();
  vtkcam->Zoom( 1.8 );
  QColor backgroundColor = QColor(0,0,0);
  int magnifierValue = 1;
  if (nullptr != renderer){
    // double a, b, c;
    // vtkcam->GetPosition(a, b, c);
    QString filter = m_PNGExtension;
    QString filePath, fileName;
    int c = 1;

    filePath = this->m_OutputDir + "axis1";
    
    //Azimuth
    vtkcam->Roll( 90 );
    vtkcam->Azimuth( -7.5 );
    
    int viewLength = 41;
    
    double horizontalDeltaDegree = 0.40;
    double verticalDeltaDegree = 0.40;

    //Horizontal Movement
    double deltaDegree = horizontalDeltaDegree;
    double currentDegree = (double)deltaDegree*(viewLength/2);
    // cout<<"currentDegree: "<<currentDegree<<endl;
    vtkcam->Azimuth( -1.0*currentDegree );

    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View1.png", "/3D_View1_");
      if(i) vtkcam->Azimuth( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }
    // cout<<"return "<<(-(double)((1+viewLength)/2)*deltaDegree)<<endl;
    vtkcam->Azimuth( -(double)((1+viewLength)/2)*deltaDegree );

    //Vertical Movement
    deltaDegree = verticalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Elevation( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View1.png", "/3D_View1_");
      if(i) vtkcam->Elevation( deltaDegree );
      // vtkcam->Elevation( (double)i*deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Elevation( -(double)((1+viewLength)/2)*deltaDegree );

    filePath = this->m_OutputDir + "axis2";
    
    //Moving to another axis
    vtkcam->Roll( 4 );
    vtkcam->Azimuth( 90 );
    vtkcam->Elevation( 4 );
    vtkcam->Roll( -90 );

    //Horizontal Movement
    deltaDegree = horizontalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Azimuth( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View2.png", "/3D_View2_");
      if(i) vtkcam->Azimuth( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Azimuth( -(double)((1+viewLength)/2)*deltaDegree );   
    
    //Vertical Movement
    deltaDegree = verticalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Elevation( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View2.png", "/3D_View2_");
      if(i) vtkcam->Elevation( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Elevation( -(double)((1+viewLength)/2)*deltaDegree );


    filePath = this->m_OutputDir + "axis3";
    
    vtkcam->Roll( 90 );
    vtkcam->Azimuth( 90 );
    vtkcam->Roll( -2.5 );
    vtkcam->Roll( 90 );
    
    //Horizontal Movement
    deltaDegree = horizontalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    // vtkcam->Roll( -90 );
    vtkcam->Azimuth( -1.0*currentDegree );
    // vtkcam->Roll( 90 );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View3.png", "/3D_View3_");
      if(i) vtkcam->Azimuth( deltaDegree );
      // double angle = vtkcam->GetRoll(); angle += 90.0;
      // vtkcam->Roll( 90 ); //vtkcam->SetRoll( angle );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Azimuth( -(double)((1+viewLength)/2)*deltaDegree );

    //Vertical Movement
    deltaDegree = verticalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Elevation( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View3.png", "/3D_View3_");
      if(i) vtkcam->Elevation( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Elevation( -(double)((1+viewLength)/2)*deltaDegree );

    filePath = this->m_OutputDir + "axis4";
    
    vtkcam->Roll( 90 );
    vtkcam->Azimuth( -90 );
    vtkcam->Roll( -90 );
    
    //Horizontal Movement
    deltaDegree = horizontalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Azimuth( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View4.png", "/3D_View4_");
      if(i) vtkcam->Azimuth( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Azimuth( -(double)((1+viewLength)/2)*deltaDegree );   

    //Vertical Movement
    deltaDegree = verticalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Elevation( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View4.png", "/3D_View4_");
      if(i) vtkcam->Elevation( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Elevation( -(double)((1+viewLength)/2)*deltaDegree );


    /*filePath = this->m_OutputDir + "axis5";

    vtkcam->Azimuth( -90 );
    
    //Horizontal Movement
    deltaDegree = horizontalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Azimuth( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View5.png", "/3D_View5_");
      if(i) vtkcam->Azimuth( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Azimuth( -(double)((1+viewLength)/2)*deltaDegree );

    //Vertical Movement
    deltaDegree = verticalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Elevation( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View5.png", "/3D_View5_");
      if(i) vtkcam->Elevation( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Elevation( -(double)((1+viewLength)/2)*deltaDegree );

    
    filePath = this->m_OutputDir + "axis6";

    vtkcam->Azimuth( 180 );
    
    //Horizontal Movement
    deltaDegree = horizontalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Azimuth( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View6.png", "/3D_View6_");
      if(i) vtkcam->Azimuth( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Azimuth( -(double)((1+viewLength)/2)*deltaDegree );   

    //Vertical Movement
    deltaDegree = verticalDeltaDegree;
    currentDegree = (double)deltaDegree*(viewLength/2);
    vtkcam->Elevation( -1.0*currentDegree );
    for(int i = 0; i < viewLength; i++){
      fileName = this->GetNewFileName(filePath, "/3D_View6.png", "/3D_View6_");
      if(i) vtkcam->Elevation( deltaDegree );
      this->TakeScreenshot(renderer, magnifierValue, filePath+fileName, filter, backgroundColor);
    }    
    vtkcam->Elevation( -(double)((1+viewLength)/2)*deltaDegree );*/
  }
}

void ScreenshotUtilities::TakeScreenshot(vtkRenderer* renderer, unsigned int magnificationFactor, QString fileName, QString filter, QColor backgroundColor){
  if ((renderer == nullptr) ||(magnificationFactor < 1) || fileName.isEmpty())
    return;

  bool doubleBuffering( renderer->GetRenderWindow()->GetDoubleBuffer() );
  renderer->GetRenderWindow()->DoubleBufferOff();

  vtkImageWriter* fileWriter = nullptr;

  QFileInfo fi(fileName);
  QString suffix = fi.suffix().toLower();

  if (suffix.isEmpty() || (suffix != "png" && suffix != "jpg" && suffix != "jpeg")){
    if (filter == m_PNGExtension){
      suffix = "png";
    }
    else if (filter == m_JPGExtension){
      suffix = "jpg";
    }
    fileName += "." + suffix;
  }

  if (suffix.compare("jpg", Qt::CaseInsensitive) == 0 || suffix.compare("jpeg", Qt::CaseInsensitive) == 0){
    vtkJPEGWriter* w = vtkJPEGWriter::New();
    w->SetQuality(100);
    w->ProgressiveOff();
    fileWriter = w;
  }
  else{ //default is png
    fileWriter = vtkPNGWriter::New();
  }

  vtkRenderLargeImage* magnifier = vtkRenderLargeImage::New();
  magnifier->SetInput(renderer);
  magnifier->SetMagnification(magnificationFactor);
  fileWriter->SetInputConnection(magnifier->GetOutputPort());
  fileWriter->SetFileName(fileName.toLatin1());

  // vtkRenderLargeImage has problems with different layers, therefore we have to
  // temporarily deactivate all other layers.
  // we set the background to white, because it is nicer than black...
  double oldBackground[3];
  renderer->GetBackground(oldBackground);

  m_BackgroundColor = backgroundColor;

  double bgcolor[] = {m_BackgroundColor.red()/255.0, m_BackgroundColor.green()/255.0, m_BackgroundColor.blue()/255.0};
  renderer->SetBackground(bgcolor);

  fileWriter->Write();
  fileWriter->Delete();
  renderer->GetRenderWindow()->SetDoubleBuffer(doubleBuffering);
}

void ScreenshotUtilities::Load(int argc, char *argv[])
{
  //*************************************************************************
  // Part I: Basic initialization
  //*************************************************************************

  this->m_DataStorageOriginal = mitk::StandaloneDataStorage::New();
  this->m_DataStorageModified = mitk::StandaloneDataStorage::New();
  bool readImage = false;
  bool readTF = false;
  bool readOuputDir = false;
  
  int i;
  for (i = 1; i < argc; ++i){
    if (strcmp(argv[i], "-i") == 0){
      readImage = true;
      readTF = false;
      readOuputDir = false;
      continue;
    }
      
    if (strcmp(argv[i], "-tf") == 0){
      readImage = false;
      readTF = true;
      readOuputDir = false;
      continue;
    }

    if (strcmp(argv[i], "-o") == 0){
      readImage = false;
      readTF = false;
      readOuputDir = true;
      continue;
    }

    if(readImage){
      mitk::StandaloneDataStorage::SetOfObjects::Pointer dataNodes = mitk::IOUtil::Load(argv[i], *this->m_DataStorageOriginal);
      if (dataNodes->empty()){
        fprintf(stderr, "Could not open file %s \n\n", argv[i]);
        exit(2);
      }
    }
    if(readTF){
      this->m_TransferFunctionFile=argv[i];
    }
    if(readOuputDir){
      this->m_OutputDir=argv[i];
    }
  }
}


/**
 \example ScreenshotUtilities.cpp
 */
