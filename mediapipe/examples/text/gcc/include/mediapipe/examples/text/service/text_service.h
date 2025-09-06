#ifndef MEDIAPIPE_EXAMPLES_TEXT_SERVICE_H_
#define MEDIAPIPE_EXAMPLES_TEXT_SERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

int mediapipe_text_join_start(void (*callback)(const char*));

int mediapipe_text_join_post(const char* s0, const char* s1);

int mediapipe_text_join_stop();

#ifdef __cplusplus
}
#endif

#endif  // MEDIAPIPE_EXAMPLES_TEXT_SERVICE_H_
