// Tencent is pleased to support the open source community by making TNN available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "test/unit_test/layer_test/layer_test.h"
#include "test/unit_test/unit_test_common.h"
#include "test/unit_test/utils/network_helpers.h"
#include "tnn/utils/dims_vector_utils.h"

namespace TNN_NS {

class SoftmaxLayerTest : public LayerTest,
                         public ::testing::WithParamInterface<std::tuple<int, int, int, int, DataType>> {};

INSTANTIATE_TEST_SUITE_P(LayerTest, SoftmaxLayerTest,
                         ::testing::Combine(testing::Values(1), testing::Values(10, 12, 10, 12), testing::Values(10),
                                            // axis
                                            testing::Values(1, 2),
                                            // dtype
                                            testing::Values(DATA_TYPE_FLOAT)));

TEST_P(SoftmaxLayerTest, SoftmaxLayer) {
    // get param
    int batch          = std::get<0>(GetParam());
    int channel        = std::get<1>(GetParam());
    int input_size     = std::get<2>(GetParam());
    int axis           = std::get<3>(GetParam());
    DataType data_type = std::get<4>(GetParam());
    DeviceType dev     = ConvertDeviceType(FLAGS_dt);

    if (data_type == DATA_TYPE_INT8 && DEVICE_ARM != dev) {
        GTEST_SKIP();
    }

    if (channel < 2) {
        GTEST_SKIP();
    }

    // blob desc
    auto inputs_desc  = CreateInputBlobsDesc(batch, channel, input_size, 1, data_type);
    auto outputs_desc = CreateOutputBlobsDesc(1, data_type);

    // param
    SoftmaxLayerParam param;
    param.name = "Softmax";
    param.axis = axis;

    Run(LAYER_SOFTMAX, &param, nullptr, inputs_desc, outputs_desc);
}

}  // namespace TNN_NS
