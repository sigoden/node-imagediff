#include <napi.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "ssim.h"

#define SCORE_THREH 1e-3
#define COLOR_THREH 192

cv::Mat toImageMat(Napi::Buffer<uchar> jsRawImg, int flag = cv::IMREAD_COLOR)
{
    uchar *buf = jsRawImg.Data();
    size_t size = jsRawImg.Length();
    std::vector<uchar> imgBytes(buf, buf + size);
    cv::Mat img = cv::imdecode(imgBytes, flag);
    return img;
}

float diffFactor(cv::Mat img1, cv::Mat img2, uint colorThred)
{
    std::pair<cv::Mat, cv::Scalar> ssim = getMSSIM(img1, img2);

    cv::Scalar score = ssim.second;
    if (score.val[0] < SCORE_THREH && score.val[1] < SCORE_THREH && score.val[2] < SCORE_THREH && score.val[3] < SCORE_THREH)
    {
        return true;
    }

    cv::Mat diff = ssim.first;
    diff.convertTo(diff, CV_8UC3, 255.0);

    cv::Mat diff1, diff2;

    int imgSize = diff.rows * diff.cols;
    cv::threshold(diff, diff1, colorThred, 255, cv::THRESH_BINARY);
    cv::cvtColor(diff1, diff2, cv::COLOR_RGB2GRAY);
    int nonZero = cv::countNonZero(diff2);

    double dirtyFactor = (imgSize - nonZero) / double(imgSize);
    return dirtyFactor;
}

Napi::Number DiffFactor(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    Napi::Buffer<uchar> jsImg1 = info[0].As<Napi::Buffer<uchar>>();
    Napi::Buffer<uchar> jsImg2 = info[1].As<Napi::Buffer<uchar>>();

    uint colorThred = COLOR_THREH;

    if (info.Length() == 3) {
        Napi::Number jsColorThred = info[2].As<Napi::Number>();
        colorThred = jsColorThred.Int32Value();
    }

    cv::Mat img1 = toImageMat(jsImg1);
    cv::Mat img2 = toImageMat(jsImg2);

    float factor = diffFactor(img1, img2, colorThred);

    return Napi::Number::New(env, factor);
}

Napi::Buffer<char> encodeMat(Napi::Env env, cv::Mat img, Napi::Object jsEncoding) {
    cv::String ext;
    std::vector<int> params = std::vector<int>();
    if (jsEncoding.Has("extname")) {
        ext = cv::String(jsEncoding.Get("extname").As<Napi::String>().Utf8Value());
    } else {
        ext = cv::String(".jpg");
    }
    if (jsEncoding.Has("quantity")) {
        int quantity = int(jsEncoding.Get("quantity").As<Napi::Number>().Int32Value());
        if (ext == ".jpg" || ext == ".jpeg") {
            params.push_back(cv::IMWRITE_JPEG_QUALITY);
            params.push_back(quantity);
        } else if (ext == ".png") {
            params.push_back(cv::IMWRITE_PNG_COMPRESSION);
            params.push_back(quantity);
        } else if (ext == "webp") {
            params.push_back(cv::IMWRITE_WEBP_QUALITY);
            params.push_back(quantity);
        }
    }
    std::vector<uchar> buf;
    cv::imencode(ext, img, buf, params);

    Napi::Buffer<char> ret = Napi::Buffer<char>::Copy(env, reinterpret_cast<char*>(buf.data()), buf.size());
    return ret;
}


Napi::Buffer<char> DiffImage(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();

    Napi::Buffer<uchar> jsImg1 = info[0].As<Napi::Buffer<uchar>>();
    Napi::Buffer<uchar> jsImg2 = info[1].As<Napi::Buffer<uchar>>();

    Napi::Object jsEncoding;

    if (info.Length() == 3) {
        jsEncoding = info[2].As<Napi::Object>();
    } else {
        jsEncoding = Napi::Object::New(env);
    }

    cv::Mat img1 = toImageMat(jsImg1);
    cv::Mat img2 = toImageMat(jsImg2);

    std::pair<cv::Mat, cv::Scalar> ssim = getMSSIM(img1, img2);

    cv::Mat diff = ssim.first;
    diff.convertTo(diff, CV_8UC3, 255.0);

    return encodeMat(env, diff, jsEncoding);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("diffFactor", Napi::Function::New(env, DiffFactor));
    exports.Set("diffImage", Napi::Function::New(env, DiffImage));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)