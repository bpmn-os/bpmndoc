#include "mdgen.h"
#include <filesystem> // for std::filesystem

std::string nodeDetails(const Node& node, const std::string& baseName, const std::string& imageDir) {
    
  auto filename = imageDir + "/" + baseName + "-" + node.id + ".svg";
  bool hasDiagram = std::filesystem::exists(filename);
  
  if ( node.documentation.empty() && node.extensionElements.empty() && !hasDiagram ) {
    return "";
  }
  std::string result;// = "- `" + node.id + "`\n";
  if ( node.documentation.size() ) {
    result += node.documentation;
    result += "\n";
  }
  
  if ( hasDiagram ) {
    std::filesystem::path path(filename);
    result += "![" + node.id + "](" + path.lexically_normal().string() + ")\n";
    result += "\n";
  }
  
  if ( node.extensionElements.size() ) {
    result += "```xml\n";
    result += node.extensionElements;
    result += "```\n";
    result += "\n";
  }

  return result;
}

void createDocumentation(std::ostream& output, const Node& node, const std::string& baseName, const std::string& imageDir) {
//std::cerr << "> Create doc for " << node.id << std::endl;
    if ( auto doc = nodeDetails(node, baseName, imageDir);
      !doc.empty()
    ) {
      output << "## " << node.type << " `" << node.id << "`\n";
      output << doc << "\n";
    }

    for ( auto& dataObject : node.dataObjects ) {
      createDocumentation(output, dataObject, baseName, imageDir);
    }
    for ( auto& flowNode : node.flowNodes ) {
      createDocumentation(output, flowNode, baseName, imageDir);
    }
    for ( auto& eventSubProcess : node.eventSubProcesses ) {
      createDocumentation(output, eventSubProcess, baseName, imageDir);
    }
};



void mdgen(
  std::ostream& output,
  const Node& root,
  const std::string& baseName, 
  const std::string& imageDir
)
{
  output << "# " << root.type << " `" << root.id << "`\n";
  output << root.documentation << "\n";
  
  auto filename = imageDir + "/" + baseName + ".svg";
  bool hasDiagram = std::filesystem::exists(filename);
  if ( hasDiagram ) {
    output << "## Diagram\n";
    output << "![" << root.id << "](" << imageDir << "/" << baseName << ".svg" << ")\n";
    output << "\n";
  }
  
  if ( root.processes.empty() ) {
    // root represents process
    output << nodeDetails(root, baseName, imageDir) << "\n";

    for ( auto& dataObject : root.dataObjects ) {
      createDocumentation(output, dataObject, baseName, imageDir);
    }
    for ( auto& flowNode : root.flowNodes ) {
      createDocumentation(output, flowNode, baseName, imageDir);
    }
    for ( auto& eventSubProcess : root.eventSubProcesses ) {
      createDocumentation(output, eventSubProcess, baseName, imageDir);
    }
    
  }
  else {
    // root represents collaboration
    for ( auto& process : root.processes ) {
      createDocumentation(output, process, baseName, imageDir);
    }
  }
}
