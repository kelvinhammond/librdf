/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * rdf_storage.h - RDF Storage Factory and Storage interfaces and definitions
 *
 * $Id$
 *
 * (C) Dave Beckett 2000 ILRT, University of Bristol
 * http://www.ilrt.bristol.ac.uk/people/cmdjb/
 *
 *                                       
 * This program is free software distributed under either of these licenses:
 *   1. The GNU Lesser General Public License (LGPL)
 * OR ALTERNATIVELY
 *   2. The modified BSD license
 *
 * See LICENSE.html or LICENSE.txt for the full license terms.
 */


#ifndef LIBRDF_STORAGE_H
#define LIBRDF_STORAGE_H

#ifdef __cplusplus
extern "C" {
#endif


/** A storage object */
struct librdf_storage_s
{
  librdf_model *model;
  void *context;
  struct librdf_storage_factory_s* factory;
};


/** A Storage Factory */
struct librdf_storage_factory_s {
  struct librdf_storage_factory_s* next;
  char* name;
  
  /* the rest of this structure is populated by the
     storage-specific register function */
  size_t context_length;
  
  /* create a new storage */
  int (*init)(librdf_storage* storage, librdf_hash* options);
  
  /* open a new storage associated with model */
  int (*open)(librdf_storage* storage, librdf_model* model);
  
  /* close a storage context inside the storage */
  int (*close)(librdf_storage* storage);
  
  /* return the number of statements in the storage for model */
  int (*size)(librdf_storage* storage);
  
  /* add a statement to the storage from the given model */
  int (*add_statement)(librdf_storage* storage, librdf_statement* statement);
  
  /* add a statement to the storage from the given model */
  int (*add_statements)(librdf_storage* storage, librdf_stream* statement_stream);
  
  /* remove a statement from the storage  */
  int (*remove_statement)(librdf_storage* storage, librdf_statement* statement);
  
  /* true if statement in storage  */
  int (*contains_statement)(librdf_storage* storage, librdf_statement* statement);
  
  /* serialise the model in storage  */
  librdf_stream* (*serialise)(librdf_storage* storage);
  
  /* serialise the results of a query */
  librdf_stream* (*find_statements)(librdf_storage* storage, librdf_statement* statement);

  /* return a list of Nodes marching given arc, target */
  librdf_iterator* (*find_sources)(librdf_storage* storage, librdf_node *arc, librdf_node *target);

  /* return a list of Nodes marching given source, target */
  librdf_iterator* (*find_arcs)(librdf_storage* storage, librdf_node *source, librdf_node *target);

  /* return a list of Nodes marching given source, target */
  librdf_iterator* (*find_targets)(librdf_storage* storage, librdf_node *source, librdf_node *target);
};

typedef struct librdf_storage_factory_s librdf_storage_factory;



/* module init */
void librdf_init_storage(void);

/* module terminate */
void librdf_finish_storage(void);

/* class methods */
void librdf_storage_register_factory(const char *name, void (*factory) (librdf_storage_factory*));
librdf_storage_factory* librdf_get_storage_factory(const char *name);

/* constructor */
librdf_storage* librdf_new_storage(char* name, char* options_string);
librdf_storage* librdf_new_storage_from_factory(librdf_storage_factory* factory, librdf_hash* options);

/* destructor */
void librdf_free_storage(librdf_storage *storage);


/* methods */
int librdf_storage_open(librdf_storage* storage, librdf_model *model);
int librdf_storage_close(librdf_storage* storage);
int librdf_storage_get(librdf_storage* storage, void *key, size_t key_len, void **value, size_t* value_len, unsigned int flags);

int librdf_storage_size(librdf_storage* storage);
int librdf_storage_add_statement(librdf_storage* storage, librdf_statement* statement);
int librdf_storage_add_statements(librdf_storage* storage, librdf_stream* statement_stream);
int librdf_storage_remove_statement(librdf_storage* storage, librdf_statement* statement);
int librdf_storage_contains_statement(librdf_storage* storage, librdf_statement* statement);
librdf_stream* librdf_storage_serialise(librdf_storage* storage);
librdf_stream* librdf_storage_find_statements(librdf_storage* storage, librdf_statement* statement);
librdf_iterator* librdf_storage_get_sources(librdf_storage *storage, librdf_node *arc, librdf_node *target);
librdf_iterator* librdf_storage_get_arcs(librdf_storage *storage, librdf_node *source, librdf_node *target);
librdf_iterator* librdf_storage_get_targets(librdf_storage *storage, librdf_node *source, librdf_node *arc);


#ifdef __cplusplus
}
#endif

#endif
