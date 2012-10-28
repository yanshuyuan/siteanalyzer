#ifndef _WEBGRAPH_H_
#define _WEBGRAPH_H_

#include <string.h>
#include <malloc.h>
#include <math.h>
#include "dict.h"
#include "hash.h"
#include "list.h"
#include "element.h"

#define VERTEX_DEFAULT 200000
#define HASH_SIZE 1<<18
#define PAGE_RANK_PRATE 1e-4
#define DAMPING_FACTOR 0.15 
#define PR_TOP 10
#define IND_TOP 10


typedef struct _vertex_info {
    int vid; /* vertex id */
    int in; /* vertex crawled or not */
    sds_t url; /* vertex url */
} vertex_info_t;

typedef struct _vertex_index {
    int vid; /* self vid */
    vertex_info_t *addr; /* vid's vertex_info in vertex_index address */
} vertex_index_t;

typedef struct _vertex {
    vertex_index_t index; 
    int ind; /* indegree */
    int outd; /*outdegree */
    double pr; /* page rank value */
    list_t edge; /* list<vertex_info_t **> vid*/
} vertex_t;

typedef struct _web_graph {
    int size;
    int vertex_size;
    vertex_t *vertex;
    dict_t vertex_dict; /*dict <vertex_info_t> */
} webg_t;


void init_webg(webg_t *web_g);
int insert_vertex(webg_t *web_g, char *element);
vertex_info_t *find_vertex_info(webg_t *web_g, char *element);
vertex_info_t *insert_vertex_info(webg_t *web_g, char *element);
vertex_t *find_vertex(webg_t *web_g, char *element);
void insert_edge(webg_t *web_g, char *url, list_t *link_url);
int vertex_size(webg_t *web_g);
void calc_degree(webg_t *web_g);
void calc_pagerank(webg_t *web_g);
void gen_ind_cdf(webg_t *web_g);
void gen_graphviz(webg_t *web_g);
void gen_ind_pagerank(webg_t *web_g, char *path);
void gen_top(webg_t *web_g);
void destroy_webg(webg_t *web_g);
void printWebg(webg_t *webgraph);

#endif

