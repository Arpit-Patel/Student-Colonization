#ifndef _RESOURCETYPE_H_
#define _RESOURCETYPE_H_

#include <vector>

enum class ResourceType { Netflix, Caffeine, Lab, Study, Lecture, Tutorial };
const std::vector<ResourceType> RESOURCE_VECTOR{ResourceType::Netflix, ResourceType::Caffeine, ResourceType::Lab, ResourceType::Study, ResourceType::Lecture, ResourceType::Tutorial};

#endif
