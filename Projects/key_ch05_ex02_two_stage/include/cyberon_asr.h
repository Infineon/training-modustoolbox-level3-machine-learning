/*
 * Copyright 2022 Cyberon Corporation
 *
 * you may not use this file except in compliance with the License.
 * A copy of the license is located in the "LICENSE" file accompanying this source.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef CYBERON_ASR_H_
#define CYBERON_ASR_H_

#include "DSpotterSDKApi.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*cyberon_asr_callback)(const char *lpchFunction, char *lpchMessage, char *lpchParameter);

/** Initialize automatic speech recognition
 *
 * @param[in] lpfnCallback  The callback of automatic speech recognition
 * @return The status of initialization
 */
BOOL cyberon_asr_init(cyberon_asr_callback lpfnCallback);

/** Do automatic speech recognition
 *
 * @param[in] lpsSample  The pointer of voice data buffer
 * @param[in] nNumSample  The number of voice data (a unit is a short, we prefer to add 480 samples per call)
 */
void cyberon_asr_process(short *lpsSample, int nNumSample);

/** Release automatic speech recognition
 *
 */
void cyberon_asr_release();

#if defined(__cplusplus)
}
#endif

#endif /* CYBERON_ASR_H_ */
