#include <string.h>

#include "glm/gtc/matrix_transform.hpp"

#include "SceneSwitcher.h"
#include "Scenes.h"

VkResult Scene0_UpdateUniformBuffer(void)
{
    GlobalContext_Switcher::MyUniformData myUniformData;
    memset(&myUniformData, 0, sizeof(GlobalContext_Switcher::MyUniformData));

    myUniformData.modelMatrix = glm::mat4(1.0f);

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));

    glm::mat4 rotationMatrix_X = glm::rotate(glm::mat4(1.0f), glm::radians(gCtx_Switcher.angle), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationMatrix_Y = glm::rotate(glm::mat4(1.0f), glm::radians(gCtx_Switcher.angle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationMatrix_Z = glm::rotate(glm::mat4(1.0f), glm::radians(gCtx_Switcher.angle), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotationMatrix = rotationMatrix_X * rotationMatrix_Y * rotationMatrix_Z;

    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.05f, 1.25f, 1.25f));

    myUniformData.modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
    myUniformData.viewMatrix = glm::mat4(1.0f);

    glm::mat4 perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f),
                                                             (float)gCtx_Switcher.winWidth / (float)gCtx_Switcher.winHeight,
                                                             0.1f,
                                                             100.0f);
    perspectiveProjectionMatrix[1][1] *= -1.0f;
    myUniformData.projectionMatrix = perspectiveProjectionMatrix;
    myUniformData.fade = glm::vec4(gCtx_Switcher.gFade, 0.0f, 0.0f, 0.0f);

    void* data = NULL;
    VkResult vkResult = vkMapMemory(gCtx_Switcher.vkDevice,
                                    gCtx_Switcher.uniformData_scene0.vkDeviceMemory,
                                    0,
                                    sizeof(GlobalContext_Switcher::MyUniformData),
                                    0,
                                    &data);
    if (vkResult != VK_SUCCESS)
    {
        fprintf(gCtx_Switcher.gpFile, "Scene0_UpdateUniformBuffer() --> vkMapMemory() failed %d\n", vkResult);
        return vkResult;
    }

    memcpy(data, &myUniformData, sizeof(GlobalContext_Switcher::MyUniformData));
    vkUnmapMemory(gCtx_Switcher.vkDevice, gCtx_Switcher.uniformData_scene0.vkDeviceMemory);
    return VK_SUCCESS;
}

