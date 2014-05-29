"""
Generate C code for translating strings into enum values at run time.
"""

import sys
import os.path

if __name__ == "__main__":
  if len(sys.argv) != 4:
    print "Usage: string_to_enum <header.h> <func_name> <target_dir>"
    exit(1)
  _, header_file, func_name, target_dir = sys.argv

  c_file = header_file[:-1] + "c"
  c_file = os.path.join(target_dir, c_file)
  print "Generating string to enum lookup from %s into file %s" % (header_file,
      c_file)

  enum_values = []
  with open(header_file) as fp:
    line = fp.readline()
    # I'm assuming the enum decleration and closing bracket are on their own
    # lines.
    while not line.startswith('enum'):
      line = fp.readline()
    return_type = " ".join(line.split()[0:2])
    line = fp.readline()
    while '}' not in line:
      line = line.strip()
      if line.endswith(','):
        line = line[:-1]
      enum_values.extend(line.split(", "))
      line = fp.readline()

  if_template = 'if (strcmp(input_string, "%(enum_value)s") == 0) {\n' \
      '  return %(enum_value)s;\n}'

  if_statements = []
  for value in enum_values:
    if_statements.append(if_template % {'enum_value': value})

  if_block = "\n".join(if_statements)

  func_template = """
/*
 * GENERATED FILE, DO NOT EDIT
 * (also, do not add to git)
 */
#include <string.h>
#include "%(header_file)s"

%(return_type)s %(func_name)s (const char *input_string) {
%(if_block)s
}"""

  with open(c_file, "w") as fp:
    fp.write(func_template % {
        'header_file': header_file,
        'return_type': return_type,
        'func_name': func_name,
        'if_block': if_block})

