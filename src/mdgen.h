#pragma once
#include "node.h"

void mdgen(
  std::ostream& output,
  const Node& root,
  const std::string& baseName, 
  const std::string& imageDir 
);
