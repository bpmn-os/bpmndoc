#include <iostream>
#include <unistd.h> // for getopt
#include <filesystem> // for std::filesystem
#include <fstream> // for file streams
#include <cassert> 

#include <bpmn++.h>
#include "node.h"
#include "mdgen.h"

void usage(const char *programName) {
  std::cerr << "Usage: " << programName << " <fileName> [-o <outputDir>] [-i <imageDir>]" << std::endl;
  std::cerr << "Options:" << std::endl;
  std::cerr << "  -o <outputDir>: Specify the output directory" << std::endl;
  std::cerr << "  -i <imageDir>: Specify the image directory" << std::endl;
  std::cerr << "  -h: Display this help message" << std::endl;
}


std::string getBasename(const std::string& path) {
    std::filesystem::path filePath(path);
    return filePath.stem().string();
}


bool createDirectory(const std::string& dir) {
  try {
    if (!std::filesystem::exists(dir)) {
      std::filesystem::create_directory(dir);
      return true;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error creating directory: " << e.what() << std::endl;
  }
  return false;
}

void createDocumentation(std::ostream& output, const std::string& baseName, const BPMN::Model& model , const std::string& imageDir) {
  assert( model.roots.size() == 1 );
  auto& root = model.roots.front();
  if ( auto collaboration = root->getOptionalChild<XML::bpmn::tCollaboration>();
    collaboration.has_value() 
  ) {
    Node node(model,collaboration.value().get());
    mdgen( output, node, baseName, imageDir );
  }
  else {
    assert( model.processes.size() == 1 );
    auto process = model.processes.front().get();
    Node node(process);
    mdgen( output, node, baseName, imageDir );
  }
}

int main(int argc, char *argv[]) {
  std::string source;
  std::string outputDir = ".";
  std::string imageDir = ".";

  int opt;
  while ((opt = getopt(argc, argv, "ho:i:")) != -1) {
    switch (opt) {
      case 'o':
       outputDir = optarg;
       break;
      case 'i':
       imageDir = optarg;
       break;
      case 'h':
       usage(argv[0]);
       return 0;
      default:
       usage(argv[0]);
       return 1;
    }
  }

  if (optind >= argc) {
    std::cerr << "Error: No model name provided." << std::endl;
    usage(argv[0]);
    return 1;
  }

  source = argv[optind];

/*
  // Now you can use modelName, outputDir, and imageDir in your program
  std::cout << "File Name: " << source << std::endl;
  std::cout << "Base Name: " << getBasename(source) << std::endl;
  std::cout << "Output Directory: " << outputDir << std::endl;
  std::cout << "Image Directory: " << imageDir << std::endl;
*/

  // Check if the output directory exists, and create it if not
  if (!std::filesystem::exists(outputDir)) {
    std::cout << "Output directory '" << outputDir << "' does not exist. Creating..." << std::endl;
    if (!createDirectory(outputDir)) {
      std::cerr << "Failed to create output directory." << std::endl;
      return 1;
    }
    std::cout << "Output directory created successfully." << std::endl;
  }
    
  BPMN::Model model(source);
  std::filesystem::path path(outputDir + "/" + getBasename(source) + ".md");
  auto target = path.lexically_normal().string();
  std::ofstream file( target );
  if (file.is_open()) { // Check if the file is opened successfully
    auto& output = file; // output now refers to the file stream
    createDocumentation(output, getBasename(source), model, imageDir);
    std::cout << "Documentation written to " << std::filesystem::absolute(target) << std::endl;
    file.close(); // Close the file stream when done
  }
  else {
    std::cerr << "Error opening output file!" << std::endl;
  }
  return 0;
}

