# Copyright 2023 Foxglove Technologies Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


def get_preset_profiles():
    return [
        ('none', 'Default profile, no special settings.'),
        ('fastwrite', 'Disables CRC and chunking for faster writing.'),
        ('zstd_fast', 'Use Zstd chunk compression on Fastest level.'),
        ('zstd_small', 'Use Zstd chunk compression on Slowest level, for smallest file size.'),
    ]
