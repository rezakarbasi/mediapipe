// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This class lets calculators allocate GpuBuffers of various sizes, caching
// and reusing them as needed. It does so by automatically creating and using
// platform-specific buffer pools for the requested sizes.
//
// This class is not meant to be used directly by calculators, but is instead
// used by GlCalculatorHelper to allocate buffers.

#ifndef MEDIAPIPE_GPU_CV_PIXEL_BUFFER_POOL_WRAPPER_H_
#define MEDIAPIPE_GPU_CV_PIXEL_BUFFER_POOL_WRAPPER_H_

#include "CoreFoundation/CFBase.h"
#include "mediapipe/gpu/gpu_buffer_format.h"
#include "mediapipe/gpu/multi_pool.h"
#include "mediapipe/gpu/pixel_buffer_pool_util.h"
#include "mediapipe/objc/CFHolder.h"

namespace mediapipe {

class CvPixelBufferPoolWrapper {
 public:
  CvPixelBufferPoolWrapper(int width, int height, GpuBufferFormat format,
                           CFTimeInterval maxAge,
                           std::function<void(void)> flush_texture_caches);

  static std::shared_ptr<CvPixelBufferPoolWrapper> Create(
      int width, int height, GpuBufferFormat format,
      const MultiPoolOptions& options,
      std::function<void(void)> flush_texture_caches = nullptr) {
    return std::make_shared<CvPixelBufferPoolWrapper>(
        width, height, format, options.max_inactive_buffer_age,
        flush_texture_caches);
  }

  CFHolder<CVPixelBufferRef> GetBuffer();

  int GetBufferCount() const { return count_; }
  std::string GetDebugString() const;

  void Flush();

  static CFHolder<CVPixelBufferRef> CreateBufferWithoutPool(
      int width, int height, GpuBufferFormat format);

 private:
  CFHolder<CVPixelBufferPoolRef> pool_;
  int count_ = 0;
  std::function<void(void)> flush_texture_caches_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_CV_PIXEL_BUFFER_POOL_WRAPPER_H_
