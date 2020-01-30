#pragma once

#include <iostream>
#include "glm.hpp"
#include "gtc/constants.hpp"
#include "Pipeline.h"
#include "PersProjInfo.h"

using namespace glm;

const mat4 initScaleTrans(vec3 scale);
const mat4 initRotateTrans(vec3 rotateInfo);
const mat4 initTranslationTrans(vec3 worldPos);
const mat4 initPersProjection(PersProjInfo persProjInfo);
const mat4 initCameraTransform(const vec3 Target, const vec3 Up);