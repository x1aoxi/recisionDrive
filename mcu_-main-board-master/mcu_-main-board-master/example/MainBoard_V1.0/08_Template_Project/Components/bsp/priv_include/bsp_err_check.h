#ifndef BSP_ERR_CHECK_H
#define BSP_ERR_CHECK_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef likely
#define likely(x)      (x)
#endif
#ifndef unlikely
#define unlikely(x)    (x)
#endif

#define BSP_ERROR_CHECK_RETURN_ERR(x) do { \
        HAL_StatusTypeDef err_rc_ = (x);            \
        if (unlikely(err_rc_ != HAL_OK)) {  \
            return err_rc_;                 \
        }                                   \
    } while(0)

#define BSP_ERROR_CHECK_RETURN_NULL(x)  do { \
        if (unlikely((x) != HAL_OK)) {      \
            return NULL;                    \
        }                                   \
    } while(0)

#define BSP_NULL_CHECK(x, ret) do { \
        if ((x) == NULL) {          \
            return ret;             \
        }                           \
    } while(0)

#define BSP_ERROR_CHECK(x, ret)      do { \
        if (unlikely((x) != HAL_OK)) {    \
            return ret;                   \
        }                                 \
    } while(0)

#define BSP_NULL_CHECK(x, ret) do { \
        if ((x) == NULL) {          \
            return ret;             \
        }                           \
    } while(0)

#define BSP_NULL_CHECK_GOTO(x, goto_tag) do { \
        if ((x) == NULL) {      \
            goto goto_tag;      \
        }                       \
    } while(0)
#endif

#ifdef __cplusplus
}
#endif
