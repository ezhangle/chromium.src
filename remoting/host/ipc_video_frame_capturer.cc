// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "remoting/host/ipc_video_frame_capturer.h"

#include "remoting/host/desktop_session_proxy.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_frame.h"
#include "third_party/webrtc/modules/desktop_capture/mouse_cursor_shape.h"

namespace remoting {

IpcVideoFrameCapturer::IpcVideoFrameCapturer(
    scoped_refptr<DesktopSessionProxy> desktop_session_proxy)
    : callback_(NULL),
      desktop_session_proxy_(desktop_session_proxy),
      capture_pending_(false),
      weak_factory_(this) {
}

IpcVideoFrameCapturer::~IpcVideoFrameCapturer() {
}

void IpcVideoFrameCapturer::Start(Callback* callback) {
  DCHECK(!callback_);
  DCHECK(callback);
  callback_ = callback;
  desktop_session_proxy_->SetVideoCapturer(weak_factory_.GetWeakPtr());
}

void IpcVideoFrameCapturer::Capture(const webrtc::DesktopRegion& region) {
  DCHECK(!capture_pending_);
  capture_pending_ = true;
  desktop_session_proxy_->CaptureFrame();
}

void IpcVideoFrameCapturer::OnCaptureCompleted(
    scoped_ptr<webrtc::DesktopFrame> frame) {
  DCHECK(capture_pending_);
  capture_pending_ = false;
  callback_->OnCaptureCompleted(frame.release());
}

}  // namespace remoting
