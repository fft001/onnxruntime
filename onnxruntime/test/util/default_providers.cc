// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <memory>
#include "default_providers.h"
#include "providers.h"
#include "core/providers/cpu/cpu_provider_factory_creator.h"
#ifdef USE_ROCM
#include "core/providers/rocm/rocm_provider_factory_creator.h"
#endif
#ifdef USE_COREML
#include "core/providers/coreml/coreml_provider_factory.h"
#endif
#include "core/session/onnxruntime_cxx_api.h"

namespace onnxruntime {
namespace test {

std::unique_ptr<IExecutionProvider> DefaultCpuExecutionProvider(bool enable_arena) {
  return CreateExecutionProviderFactory_CPU(enable_arena)->CreateProvider();
}

std::unique_ptr<IExecutionProvider> DefaultTensorrtExecutionProvider() {
#ifdef USE_TENSORRT
  OrtTensorRTProviderOptions params{
      0,
      0,
      nullptr,
      1000,
      1,
      1 << 30,
      0,
      0,
      nullptr,
      0,
      0,
      0,
      0,
      0,
      nullptr,
      0,
      nullptr,
      0};
  if (auto factory = CreateExecutionProviderFactory_Tensorrt(&params))
    return factory->CreateProvider();
#endif
  return nullptr;
}

std::unique_ptr<IExecutionProvider> DefaultMIGraphXExecutionProvider() {
#ifdef USE_MIGRAPHX
  return CreateExecutionProviderFactory_MIGraphX(0)->CreateProvider();
#else
  return nullptr;
#endif
}

std::unique_ptr<IExecutionProvider> DefaultOpenVINOExecutionProvider() {
#ifdef USE_OPENVINO
  OrtOpenVINOProviderOptions params;
  return CreateExecutionProviderFactory_OpenVINO(&params)->CreateProvider();
#else
  return nullptr;
#endif
}

std::unique_ptr<IExecutionProvider> DefaultCudaExecutionProvider() {
#ifdef USE_CUDA
  OrtCUDAProviderOptions provider_options{};
  provider_options.do_copy_in_default_stream = true;
  if (auto factory = CreateExecutionProviderFactory_Cuda(&provider_options))
    return factory->CreateProvider();
#endif
  return nullptr;
}

std::unique_ptr<IExecutionProvider> DefaultDnnlExecutionProvider(bool enable_arena) {
#ifdef USE_DNNL
  if (auto factory = CreateExecutionProviderFactory_Dnnl(enable_arena ? 1 : 0))
    return factory->CreateProvider();
#else
  ORT_UNUSED_PARAMETER(enable_arena);
#endif
  return nullptr;
}

std::unique_ptr<IExecutionProvider> DefaultNupharExecutionProvider(bool allow_unaligned_buffers) {
#ifdef USE_NUPHAR
  return CreateExecutionProviderFactory_Nuphar(allow_unaligned_buffers, "")->CreateProvider();
#else
  ORT_UNUSED_PARAMETER(allow_unaligned_buffers);
  return nullptr;
#endif
}

std::unique_ptr<IExecutionProvider> DefaultNnapiExecutionProvider() {
// For any non - Android system, NNAPI will only be used for ort model converter
// Make it unavailable here, you can still manually append NNAPI EP to session for model conversion
#if defined(USE_NNAPI) && defined(__ANDROID__)
  return CreateExecutionProviderFactory_Nnapi(0, {})->CreateProvider();
#else
  return nullptr;
#endif
}

std::unique_ptr<IExecutionProvider> DefaultRknpuExecutionProvider() {
#ifdef USE_RKNPU
  return CreateExecutionProviderFactory_Rknpu()->CreateProvider();
#else
  return nullptr;
#endif
}

std::unique_ptr<IExecutionProvider> DefaultAclExecutionProvider(bool enable_arena) {
#ifdef USE_ACL
  return CreateExecutionProviderFactory_ACL(enable_arena)->CreateProvider();
#else
  ORT_UNUSED_PARAMETER(enable_arena);
  return nullptr;
#endif
}

std::unique_ptr<IExecutionProvider> DefaultArmNNExecutionProvider(bool enable_arena) {
#ifdef USE_ARMNN
  return CreateExecutionProviderFactory_ArmNN(enable_arena)->CreateProvider();
#else
  ORT_UNUSED_PARAMETER(enable_arena);
  return nullptr;
#endif
}

std::unique_ptr<IExecutionProvider> DefaultRocmExecutionProvider() {
#ifdef USE_ROCM
  return CreateExecutionProviderFactory_ROCM(ROCMExecutionProviderInfo{})->CreateProvider();
#else
  return nullptr;
#endif
}

std::unique_ptr<IExecutionProvider> DefaultCoreMLExecutionProvider() {
#if defined(USE_COREML)
  // We want to run UT on CPU only to get output value without losing precision
  uint32_t coreml_flags = 0;
  coreml_flags |= COREML_FLAG_USE_CPU_ONLY;
  return CreateExecutionProviderFactory_CoreML(coreml_flags)->CreateProvider();
#else
  return nullptr;
#endif
}

}  // namespace test
}  // namespace onnxruntime
