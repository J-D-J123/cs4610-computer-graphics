# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MarkdownEditorProject1-ComputerGraphics_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MarkdownEditorProject1-ComputerGraphics_autogen.dir\\ParseCache.txt"
  "MarkdownEditorProject1-ComputerGraphics_autogen"
  )
endif()
