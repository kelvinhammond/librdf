/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * rdf_serializer.h - RDF Serializer Factory / Serializer interfaces and definition
 *
 * $Id$
 *
 * Copyright (C) 2002-2004, David Beckett http://purl.org/net/dajobe/
 * Institute for Learning and Research Technology http://www.ilrt.bristol.ac.uk/
 * University of Bristol, UK http://www.bristol.ac.uk/
 * 
 * This package is Free Software and part of Redland http://librdf.org/
 * 
 * It is licensed under the following three licenses as alternatives:
 *   1. GNU Lesser General Public License (LGPL) V2.1 or any newer version
 *   2. GNU General Public License (GPL) V2 or any newer version
 *   3. Apache License, V2.0 or any newer version
 * 
 * You may not use this file except in compliance with at least one of
 * the above three licenses.
 * 
 * See LICENSE.html or LICENSE.txt at the top of this package for the
 * complete terms and further detail along with the license texts for
 * the licenses in COPYING.LIB, COPYING and LICENSE-2.0.txt respectively.
 * 
 * 
 */



#ifndef LIBRDF_SERIALIZER_H
#define LIBRDF_SERIALIZER_H


#ifdef __cplusplus
extern "C" {
#endif

#ifdef LIBRDF_INTERNAL

struct librdf_serializer_factory_s 
{
  struct librdf_serializer_factory_s* next;

  /* factory name - required */
  char *name;

  /* serialize to this MIME type/ Internet Media Type - optional */
  char *mime_type;

  /* writes the syntax defined by this URI - optional */
  librdf_uri *type_uri;

  /* the rest of this structure is populated by the
     serializer-specific register function */
  size_t  context_length;

  /* create a new serializer */
  int (*init)(librdf_serializer* serializer, void *_context);

  /* destroy a serializer */
  void (*terminate)(void *_context);

  /* get/set features of serializer */
  librdf_node* (*get_feature)(void *_context, librdf_uri* feature);
  int (*set_feature)(void *_context, librdf_uri *feature, librdf_node* value);

  int (*set_namespace)(void *_context, librdf_uri *uri, const char *prefix);
  
  int (*serialize_model_to_file_handle)(void *_context, FILE *handle, librdf_uri* base_uri, librdf_model *model);

  unsigned char* (*serialize_model_to_counted_string)(void *_context, librdf_uri* base_uri, librdf_model *model, size_t *length_p);
};


struct librdf_serializer_s {
  librdf_world *world;
  
  void *context;

  void *error_user_data;
  void *warning_user_data;
  void (*error_fn)(void *user_data, const char *msg, ...);
  void (*warning_fn)(void *user_data, const char *msg, ...);

  librdf_serializer_factory* factory;
};

/* class methods */
librdf_serializer_factory* librdf_get_serializer_factory(librdf_world *world, const char *name, const char *mime_type, librdf_uri *type_uri);


/* module init */
void librdf_init_serializer(librdf_world *world);
/* module finish */
void librdf_finish_serializer(librdf_world *world);
                    
void librdf_serializer_raptor_constructor(librdf_world* world);
void librdf_serializer_rdfxml_constructor(librdf_world* world);

#endif


/* class methods */
REDLAND_API void librdf_serializer_register_factory(librdf_world *world, const char *name, const char *mime_type, const unsigned char *uri_string, void (*factory) (librdf_serializer_factory*));

/* constructor */
REDLAND_API librdf_serializer* librdf_new_serializer(librdf_world* world, const char *name, const char *mime_type, librdf_uri *type_uri);
REDLAND_API librdf_serializer* librdf_new_serializer_from_factory(librdf_world* world, librdf_serializer_factory *factory);

/* destructor */
REDLAND_API void librdf_free_serializer(librdf_serializer *serializer);


/* methods */
REDLAND_API REDLAND_DEPRECATED int librdf_serializer_serialize_model(librdf_serializer* serializer, FILE *handle, librdf_uri* base_uri, librdf_model* model);
REDLAND_API int librdf_serializer_serialize_model_to_file_handle(librdf_serializer* serializer, FILE *handle, librdf_uri* base_uri, librdf_model* model);
REDLAND_API int librdf_serializer_serialize_model_to_file(librdf_serializer* serializer, const char *name, librdf_uri* base_uri, librdf_model* model);
REDLAND_API unsigned char* librdf_serializer_serialize_model_to_string(librdf_serializer* serializer, librdf_uri* base_uri, librdf_model* model);
REDLAND_API unsigned char* librdf_serializer_serialize_model_to_counted_string(librdf_serializer* serializer, librdf_uri* base_uri, librdf_model* model, size_t *length_p);
REDLAND_API void librdf_serializer_set_error(librdf_serializer* serializer, void *user_data, void (*error_fn)(void *user_data, const char *msg, ...));
REDLAND_API void librdf_serializer_set_warning(librdf_serializer* serializer, void *user_data, void (*warning_fn)(void *user_data, const char *msg, ...));

REDLAND_API librdf_node* librdf_serializer_get_feature(librdf_serializer* serializer, librdf_uri *feature);
REDLAND_API int librdf_serializer_set_feature(librdf_serializer* serializer, librdf_uri *feature, librdf_node* value);
REDLAND_API int librdf_serializer_set_namespace(librdf_serializer* serializer, librdf_uri *uri, const char *prefix);


#ifdef __cplusplus
}
#endif

#endif
