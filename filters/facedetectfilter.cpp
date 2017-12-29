#include "facedetectfilter.h"

using namespace std;
using namespace cv;

FaceDetectFilter::FaceDetectFilter(QObject *parent): AbstractFilter(parent)
{
    face_cascade_gpu = cv::cuda::CascadeClassifier::create("/media/zaak/Data/dev/CamGrab/opencv/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml");
}

void FaceDetectFilter::apply(cv::Mat &mat)
{
    std::vector<Rect> faces;
     Mat frame_gray;
     Mat crop;
     Mat res;
     Mat gray;
     string text;
     stringstream sstm;

     cvtColor(mat, frame_gray, COLOR_BGR2GRAY);
     equalizeHist(frame_gray, frame_gray);

     cv::cuda::GpuMat frame_gpu(frame_gray);
     cv::cuda::GpuMat objbuf_gpu;

     // Detect faces
     face_cascade_gpu->detectMultiScale(frame_gpu, objbuf_gpu);
     face_cascade_gpu->convert(objbuf_gpu, faces);

     // Set Region of Interest
     cv::Rect roi_b;
     cv::Rect roi_c;

     size_t ic = 0; // ic is index of current element
     int ac = 0; // ac is area of current element

     size_t ib = 0; // ib is index of biggest element
     int ab = 0; // ab is area of biggest element

     for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

     {
         roi_c.x = faces[ic].x;
         roi_c.y = faces[ic].y;
         roi_c.width = (faces[ic].width);
         roi_c.height = (faces[ic].height);

         ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

         roi_b.x = faces[ib].x;
         roi_b.y = faces[ib].y;
         roi_b.width = (faces[ib].width);
         roi_b.height = (faces[ib].height);

         ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

         if (ac > ab)
         {
             ib = ic;
             roi_b.x = faces[ib].x;
             roi_b.y = faces[ib].y;
             roi_b.width = (faces[ib].width);
             roi_b.height = (faces[ib].height);
         }

         Point pt1(faces[ic].x, faces[ic].y); // Display detected faces on main window - live stream from camera
         Point pt2((faces[ic].x + faces[ic].height), (faces[ic].y + faces[ic].width));
         cv::rectangle(mat, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
     }
}
