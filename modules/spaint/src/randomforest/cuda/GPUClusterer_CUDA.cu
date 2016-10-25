/**
 * spaint: GPUClusterer_CUDA.cu
 * Copyright (c) Torr Vision Group, University of Oxford, 2016. All rights reserved.
 */

#include "randomforest/cuda/GPUClusterer_CUDA.h"

#include "util/MemoryBlockFactory.h"

namespace spaint
{
__global__ void ck_compute_density(const PositionColourExample *examples,
    const int *reservoirSizes, float *densities, int reservoirCapacity,
    int startReservoirIdx, float sigma)
{
  // The assumption is that the kernel indices are always valid.
  const int reservoirIdx = blockIdx.x + startReservoirIdx;
  const int reservoirOffset = reservoirIdx * reservoirCapacity;
  const int reservoirSize = reservoirSizes[reservoirIdx];
  const int elementIdx = threadIdx.x;

  const float three_sigma_sq = (3.f * sigma) * (3.f * sigma); // Points farther away have small contribution to the density
  const float minus_one_over_two_sigma_sq = -1.f / (2.f * sigma * sigma);

  float density = 0.f;

  if (elementIdx < reservoirSize)
  {
    const Vector3f centerPosition =
        examples[reservoirOffset + elementIdx].position;

    for (int i = 0; i < reservoirSize; ++i)
    {
      const Vector3f examplePosition = examples[reservoirOffset + i].position;
      const Vector3f diff = examplePosition - centerPosition;
      const float normSq = dot(diff, diff);

      if (normSq < three_sigma_sq)
      {
        density += expf(normSq * minus_one_over_two_sigma_sq);
      }
    }
  }

  densities[reservoirOffset + elementIdx] = density;
}

GPUClusterer_CUDA::GPUClusterer_CUDA(float sigma, float tau) :
    GPUClusterer(sigma, tau)
{
  MemoryBlockFactory &mbf = MemoryBlockFactory::instance();
  m_densities = mbf.make_image<float>(Vector2i(0, 0));
}

void GPUClusterer_CUDA::find_modes(const PositionReservoir_CPtr &reservoirs,
    GPUForestPredictionsBlock_Ptr &predictions, size_t startIdx, size_t count)
{
  const int nbReservoirs = reservoirs->get_reservoirs_count();
  const int reservoirCapacity = reservoirs->get_capacity();

  if (startIdx + count > nbReservoirs)
    throw std::runtime_error("startIdx + count > nbReservoirs");

  m_densities->ChangeDims(Vector2i(reservoirCapacity, nbReservoirs)); // Happens only once

  const PositionColourExample *examples = reservoirs->get_reservoirs()->GetData(
      MEMORYDEVICE_CUDA);
  const int *reservoirSizes = reservoirs->get_reservoirs_size()->GetData(
      MEMORYDEVICE_CUDA);
  float *densities = m_densities->GetData(MEMORYDEVICE_CUDA);

  dim3 blockSize(reservoirCapacity); // One thread per item in each reservoir
  dim3 gridSize(count); // One block per reservoir to process
  ck_compute_density<<<gridSize, blockSize>>>(examples, reservoirSizes, densities, reservoirCapacity, startIdx, m_sigma);
  cudaDeviceSynchronize();
}

}
