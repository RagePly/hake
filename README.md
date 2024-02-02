# h(ugo's m)ake

A "desired state configuration tool", built in C.

## Goals

- Module System
  - Compile goals/strategies to shared objects that are linked dynamically on demand
  - Link/Unlink based on usage
- Deamon
  - Run __hake__ as a service
  - Configure/Update the desired state of the machine
  - Query for status/system health
  - Upload new/upgraded modules to be used without stopping the process through the module system
  - __hake__ will periodically verify the current state and issue actions to maintain it
- Dependencies
  - Define the desired state as a dependency on several goals
  - __hake__ will create dependency graphs and schedule strategies in sequence or parallel, where possible
- Meta
  - Modules should utilize existing software/solutions when defining strategies, no need to reinvent the wheel
  - Rather than reimplementing behaviour, modules should create interfaces and interact with api's/abi's
- Distributed
  - Different __hake__ processes across different environments (machine, lan, web) are managed in an hierarchical structure
  with a custom protocol
  - Allows one to configure a network of environments from a single instance that relays instructions to other instances 
  - A desired state is to have __hake__ installed in another environment, it is principal that it should know how to replicate and install itself

