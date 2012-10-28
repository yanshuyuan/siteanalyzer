#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "webgraph.h"
#include "list.h"

int main() 
{
    char *url1 = "http://125.211.218.8/techqq/index.html";
    list_t link_url1;
    list_t link_url3;
    list_init(&link_url1);
    list_init(&link_url3);
    char *url2 = "http://125.211.218.8/techqq/index.html";
    char *url3 = "http://125.211.218.8/techqq/a/00001.htm";
    char *url4 = "http://125.211.218.8/techqq/b/00001.htm";
    char *url5 = "http://125.211.218.8/techqq/wfy.htm";
    char *url6 = "http://125.211.218.8/techqq/wfy1.htm";
    list_push_back(&link_url1, url2, strlen(url2));
    list_push_back(&link_url1, url3, strlen(url3));
    list_push_back(&link_url1, url4, strlen(url4));
    list_push_back(&link_url1, url5, strlen(url5));
    
	
    webg_t webgraph;
    init_webg(&webgraph);
    insert_edge(&webgraph, url1, &link_url1);
    insert_edge(&webgraph, url3, &link_url3);
    

    int i;
    printf("vertex size: %d\n", webgraph.vertex_size);
    for(i = 0; i < webgraph.vertex_size; i++) {
	printf("v[%d].index.vid = %d\n", i, webgraph.vertex[i].index.vid);
	printf("v[%d].index.addr = %p\n", i, webgraph.vertex[i].index.addr);
	printf("vinfo[%d].in = %d\n", i, webgraph.vertex[i].index.addr->in);
	printf("vinfo[%d].vid = %d\n", i, webgraph.vertex[i].index.addr->vid);
	printf("vinfo[%d].ulen = %d\n", i, webgraph.vertex[i].index.addr->url.len);
	printf("vinfo[%d].url = %s\n", i, webgraph.vertex[i].index.addr->url.buffer);
	elem_t *iter;
	int ix;
	for(iter = list_first(&webgraph.vertex[i].edge); iter != list_end(&webgraph.vertex[i].edge);
		iter = list_next(iter)) {
	    printf("iter->len: %d\n", iter->len);
	    vertex_info_t *x = *(vertex_info_t **)iter->buffer;
	    printf("edge[%d].vinfo.in: %d\n", ix, x->in);
	    printf("edge[%d].vinfo.vid: %d\n", ix, x->vid);
	    printf("edge[%d].vinfo.ulen: %d\n", ix, x->url.len);
	    printf("edge[%d].vinfo.url: %s\n", ix, x->url.buffer);
	    ix++;
	}
    }
    printf("=======================================\n");

    assert(webgraph.vertex_size == 2);


    vertex_t *v1 = find_vertex(&webgraph, url1);
    assert(v1 != NULL);
    assert(list_size(&v1->edge) == 4);
    assert(v1->index.vid == 0);
    assert(v1->index.addr == find_vertex_info(&webgraph, url1));
    vertex_info_t *vinfo1 = find_vertex_info(&webgraph, url1);
    assert(vinfo1 != NULL);
    assert(vinfo1->vid == 0);
    assert(vinfo1->in == 1);
    assert(strcmp(vinfo1->url.buffer, url1) == 0);
    

    vertex_t *v2 = find_vertex(&webgraph, url2);
    assert(list_size(&v2->edge) == 4);
    assert(v2->index.vid == 0);
    assert(v2->index.addr == find_vertex_info(&webgraph, url2));
    assert(v2 != NULL);
    vertex_info_t *vinfo2 = find_vertex_info(&webgraph, url2);
    assert(vinfo2 != NULL);
    assert(vinfo2->vid == 0);
    assert(vinfo2->in == 1);
    assert(strcmp(vinfo2->url.buffer, url2) == 0);
	

    vertex_t *v3 = find_vertex(&webgraph, url3);
    assert(v3 != NULL);
    assert(list_size(&v3->edge) == 0);
    assert(v3->index.vid == 1);
    assert(v3->index.addr == find_vertex_info(&webgraph, url3));
    vertex_info_t *vinfo3 = find_vertex_info(&webgraph, url3);
    assert(vinfo3 != NULL);
    assert(vinfo3->vid == 1);
    assert(vinfo3->in == 1);
    assert(strcmp(vinfo3->url.buffer, url3) == 0);

    vertex_t *v4 = find_vertex(&webgraph, url4);
    assert(v4 == NULL);
    vertex_info_t *vinfo4 = find_vertex_info(&webgraph, url4);
    assert(vinfo4 != NULL);
    assert(vinfo4->vid == -1);
    assert(vinfo4->in == 0);
    assert(strcmp(vinfo4->url.buffer, url4) == 0);

    vertex_t *v5 = find_vertex(&webgraph, url5);
    assert(v5 == NULL);
    vertex_info_t *vinfo5 = find_vertex_info(&webgraph, url5);
    assert(vinfo5 != NULL);
    assert(vinfo5->vid == -1);
    assert(vinfo5->in == 0);
    assert(strcmp(vinfo5->url.buffer, url5) == 0);


    vertex_t *v6 = find_vertex(&webgraph, url6);
    assert(v6 == NULL);
    vertex_info_t *vinfo6 = find_vertex_info(&webgraph, url6);
    assert(vinfo6 == NULL);

    printWebg(&webgraph);

    list_destroy(&link_url1);
    list_destroy(&link_url3);
    destroy_webg(&webgraph);

}
