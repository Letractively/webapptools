
/*
  DO NOT EDIT!
  This file has been generated by generate_sources.py script.
  $Id$
*/

#include <html_js.h>
using namespace v8;
v8::Handle<v8::Value> js_traversal_DocumentTraversal::createNodeIterator(v8::Handle<v8::Value> val_root, long unsigned int val_whatToShow, v8::Handle<v8::Value> val_filter, bool val_entityReferenceExpansion)
{
    return traversal::DocumentTraversal::createNodeIterator(val_root, val_whatToShow, val_filter, val_entityReferenceExpansion);
}
v8::Handle<v8::Value> js_traversal_DocumentTraversal::createTreeWalker(v8::Handle<v8::Value> val_root, long unsigned int val_whatToShow, v8::Handle<v8::Value> val_filter, bool val_entityReferenceExpansion)
{
    return traversal::DocumentTraversal::createTreeWalker(val_root, val_whatToShow, val_filter, val_entityReferenceExpansion);
}
