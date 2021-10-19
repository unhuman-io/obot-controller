/* Demonstrates how to add custom attributes */

#include "gcc-plugin.h"
#include <stdlib.h>
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-pass.h"
#include "intl.h"
#include "toplev.h"
#include "plugin.h"
#include "diagnostic.h"
#include "stringpool.h"
#include "attribs.h"
#include <iostream>
using namespace std;

int plugin_is_GPL_compatible;

static struct attribute_spec tuneable_attr =
  { "tuneable", 0, 1, false,  false, false, false, NULL, NULL };

/* Plugin callback called during attribute registration */

static void register_attributes (void *event_data, void *data) 
{
  register_attribute (&tuneable_attr);
}

void parse_field_decl (tree field, std::string header) {
  std::string prefix = header;
  if (prefix != "") {
     prefix += ".";
  }
  if (TREE_CODE(TREE_TYPE(field)) == RECORD_TYPE) {
     for ( tree tf = TYPE_FIELDS(TREE_TYPE(field)); tf; tf = TREE_CHAIN(tf)) {
          if (TREE_CODE(tf) == FIELD_DECL) {
              //cerr << IDENTIFIER_POINTER(DECL_NAME(field));
              if (DECL_NAME(field)) {
                parse_field_decl(tf, prefix + IDENTIFIER_POINTER(DECL_NAME(field)));
              }
          }
      }
    return;
  }
//   cerr << get_tree_code_name(TREE_CODE(field)) << " " << 
//     get_tree_code_name(TREE_CODE(TREE_TYPE(field))) << " " <<
//     IDENTIFIER_POINTER(DECL_NAME(field)) << endl;

  for (tree attr = DECL_ATTRIBUTES (field); attr; attr = TREE_CHAIN (attr)) {
    tree attrname = TREE_PURPOSE (attr);
    tree attrargs = TREE_VALUE (attr);
    if(lookup_attribute("tuneable", DECL_ATTRIBUTES(field))) {
        tree type = TREE_TYPE(field);
        tree type_name_tree = TYPE_NAME(type);
        std::string type_name;
        std::string unsign = TYPE_UNSIGNED(type) ? "U" : "";        
        std::string size = std::to_string(TYPE_PRECISION(type));
        std::string const_str = TREE_READONLY(TREE_TYPE(field)) ? "const" : "";
        switch (TREE_CODE(type)) {
            case INTEGER_TYPE:
                type_name = unsign + "Int" + size;
                break;
            case REAL_TYPE:
                type_name = "Float";
                break;
            case BOOLEAN_TYPE:
                type_name = "Bool";
                break;
        }


      cerr << "tuneable " << IDENTIFIER_POINTER(DECL_NAME(type_name_tree)) << (TREE_READONLY(TREE_TYPE(field)) ? " const" : "") << std::endl;

      cout << const_str << " " << type_name << " " << prefix + IDENTIFIER_POINTER(DECL_NAME(field)) << std::endl;
    }
  }
}

void finish_type_callback (void*event_data, void*data)
{
  //cerr << "finish decl" << std::endl;
  tree decl = (tree)event_data;
  if (decl == NULL_TREE || decl == error_mark_node) {
		return;
  }
   //cerr << get_tree_code_name(TREE_CODE(decl)) << " " << IDENTIFIER_POINTER(DECL_NAME(decl)) << std::endl;

  tree type = TREE_TYPE(decl);

    if (TREE_CODE(decl) == VAR_DECL && TREE_CODE(type) == RECORD_TYPE) {
     // cerr << get_tree_code_name(TREE_CODE(type)) << " " << IDENTIFIER_POINTER(DECL_NAME(decl)) << endl;
            parse_field_decl(decl, "");
      
    }
}

int plugin_init (struct plugin_name_args *plugin_info,
             struct plugin_gcc_version *version)
{
  const char *plugin_name = plugin_info->base_name;
  register_callback(plugin_name,
        PLUGIN_FINISH_DECL, &finish_type_callback, 0);
  register_callback (plugin_name, PLUGIN_ATTRIBUTES, register_attributes, NULL);
  return 0;
}
