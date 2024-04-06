#pragma once

#include <list>
#include <string>
#include <bpmn++.h>

struct Node {
  std::string id;
  std::string type;
  std::string documentation;
  std::string extensionElements;
  std::list<Node> processes;
  std::list<Node> sequenceFlows;
  std::list<Node> messageFlows;
  std::list<Node> dataObjects;
  std::list<Node> flowNodes;
  std::list<Node> eventSubProcesses;
  Node(const BPMN::Model& model, XML::bpmn::tCollaboration& collaboration);
  Node(BPMN::Process* process);
  Node(BPMN::EventSubProcess* eventSubProcess);
  Node(BPMN::FlowNode* flowNode);
  Node(BPMN::DataObject* dataObject);
  Node(BPMN::SequenceFlow* sequenceFlow);
  Node(BPMN::MessageFlow* messageFlow);
private:
  std::string getType(BPMN::FlowNode* flowNode);
};
