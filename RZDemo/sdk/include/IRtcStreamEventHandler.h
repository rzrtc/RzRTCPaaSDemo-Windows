//
// Created by yhy on 11/4/20.
//

#ifndef PAASSDK_IRTCSTREAMEVENTHANDLER_H
#define PAASSDK_IRTCSTREAMEVENTHANDLER_H

#include "IRtcEngineState.h"

namespace rz {

class IRtcStream;

class IRtcStreamEventHandler {
 public:
  virtual void onVideoPublishStateChanged(IRtcStream *stream, PUBLISH_STREAM_STATE state, int elapsed) {
    (void)stream;
    (void)state;
    (void)elapsed;
  }

  virtual void onLocalVideoStateChanged(IRtcStream *stream, LOCAL_VIDEO_STREAM_STATE state, LOCAL_VIDEO_STREAM_ERROR error, int elapsed) {
    (void)stream;
    (void)state;
    (void)error;
    (void)elapsed;
  }

  virtual void onFirstVideoFramePublished(IRtcStream *stream, int elapsed) {
    (void)stream;
    (void)elapsed;
  }

  virtual void onVideoSizeChanged(IRtcStream *stream, int width, int height) {
    (void)stream;
    (void)width;
    (void)height;
  }

  virtual void onLocalVideoStats(IRtcStream* stream, const LocalVideoStats& stats) {
      (void)stream;
      (void)stats; 
  }

  virtual void onPredictedBitrateChanged(IRtcStream* stream, int newBitrate, bool isLow) { 
      (void)stream;
      (void)newBitrate;
      (void)isLow;
  };
};

}  // namespace rz

#endif  // PAASSDK_IRTCSTREAMEVENTHANDLER_H
