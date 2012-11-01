/*****
** webgraph.c
** - implements the methods declared in webgraph.h
** - 构造、维护和存储整个web graph的顶点和边数据,分析web graph的链接关系,确保多线程安全访问
*/
#include "webgraph.h"

void init_webg(webg_t *web_g)
{
    web_g->vertex = (vertex_t *) calloc(sizeof(vertex_t), VERTEX_DEFAULT);
    if(web_g->vertex != NULL) {
	int i;
	for(i = 0; i < VERTEX_DEFAULT; i++) {
	    list_init(&web_g->vertex[i].edge);
	    web_g->vertex[i].index.vid = i;
	    web_g->vertex[i].index.addr = NULL;
	    web_g->vertex[i].pr = 0.0;
	}
	web_g->size = HASH_SIZE;
	web_g->vertex_size = 0;
    	dict_init(&web_g->vertex_dict, HASH_SIZE, sdbm_hash);
    } else {
	fprintf(stderr, "Error, allocate memory failed in func 'init_webg'.\n");
    }
}

vertex_info_t *insert_vertex_info(webg_t *web_g, char *element)
{
    vertex_info_t *vertex_info = find_vertex_info(web_g, element);
    /* not find in vertex index, so need insert*/
    if(vertex_info == NULL) {
	int len = strlen(element);
	int blen = len + 1; /* copy '\0' */
	vertex_info_t *vinfo = (vertex_info_t *)malloc(sizeof(vertex_info_t) + blen);
	if(vinfo != NULL) {
	    /* create vertex info and add into vertex dict */
	    vinfo->vid = -1; /* we don't known the edge exists or not, so vid = -1 means not exists, when crawl the url success will change it */
	    vinfo->in = 0; /* init need set in to 0, means the crawl not crawl, if add job in do_work, it will change to 1 */
	    vinfo->url.len = blen;
	    bzero(vinfo->url.buffer, blen);
	    memcpy(vinfo->url.buffer, element, len);
	    vertex_info = (vertex_info_t *)dict_insert(&web_g->vertex_dict, element, len, vinfo, sizeof(vertex_info_t) + blen);
	    free(vinfo);
	} else {
	    fprintf(stderr, "Error, allocate memory failed in func 'insert_vertex'.\n");
	}
    }
    return vertex_info;

}

int insert_vertex(webg_t *web_g, char *element)
{
    vertex_info_t *vertex_info = find_vertex_info(web_g, element);
    /* not find in vertex index, so need insert*/
    if(vertex_info == NULL) {
	int len = strlen(element);
	int blen = len + 1; /* copy '\0' */
	vertex_info_t *vinfo = (vertex_info_t *)malloc(sizeof(vertex_info_t) + blen);
	if(vinfo != NULL) {
	    /* create vertex info and add into vertex dict */
	    vinfo->in = 1;
	    vinfo->vid = web_g->vertex_size;	
	    vinfo->url.len = blen;
	    bzero(vinfo->url.buffer, blen);
	    memcpy(vinfo->url.buffer, element, len);
	    vertex_info = (vertex_info_t *)dict_insert(&web_g->vertex_dict, element, len, vinfo, sizeof(vertex_info_t) + blen);
    	    free(vinfo);
	} else {
	    fprintf(stderr, "Error, allocate memory failed in func 'insert_vertex'.\n");
	}
    }  else {
	vertex_info->in = 1;
	vertex_info->vid = web_g->vertex_size;
    }
    /* init vertex */
    vertex_t *vtex = &web_g->vertex[web_g->vertex_size];
    vtex->index.addr = vertex_info;
    web_g->vertex_size++;
    return vertex_info->vid;
}

vertex_info_t *find_vertex_info(webg_t *web_g, char *element)
{
    int len = strlen(element);
    void *vinfo = dict_find(&web_g->vertex_dict, element, len); 
    return vinfo == NULL ? NULL : (vertex_info_t *)vinfo;
}

vertex_t *find_vertex(webg_t *web_g, char *element)
{
    vertex_info_t *vinfo = find_vertex_info(web_g, element);
    return vinfo == NULL ? NULL : (vinfo->vid < 0 ? NULL : &web_g->vertex[vinfo->vid]);
}

void insert_edge(webg_t *web_g, char *url, list_t *link_url)
{
    /* get url info */
    int vid;
    vertex_t *vtex = find_vertex(web_g, url);
    if(vtex == NULL) { /* if no url vertex, then create it*/
	vid = insert_vertex(web_g, url);
	vtex = &web_g->vertex[vid];
    } else {
	vid = vtex->index.vid;
	fprintf(stderr, "If run code in this, you must crawl the same url more than twice.\n");
    }
    elem_t *iter;
    vertex_info_t *evtex_info;
    for(iter = list_first(link_url); iter != list_end(link_url); iter = list_next(iter)) {
	/* find edge vertex info */
	evtex_info = insert_vertex_info(web_g, iter->buffer);
	/* add edge vid to vtex edge */
	list_push_back(&vtex->edge, &evtex_info, sizeof(vertex_info_t **));
    }
}

int vertex_size(webg_t *web_g)
{
    return web_g->vertex_size;
}

int _pr_cmp(vertex_t **a, vertex_t **b) 
{
    return (*a)->pr < (*b)->pr;
}

int _ind_cmp(vertex_t **a, vertex_t **b) 
{
    return (*a)->ind < (*b)->ind;
}

void gen_top(webg_t *web_g)
{
    vertex_t **vertex_ptr;
    int i;
    FILE *fp = fopen("top10.dat", "w");
    if(fp != NULL) {
        if((vertex_ptr = (vertex_t **) malloc(sizeof(vertex_t *) * web_g->vertex_size)) != NULL) {
            for(i = 0; i < web_g->vertex_size; i++) {
                vertex_ptr[i] = &web_g->vertex[i];
            }
            qsort(vertex_ptr, web_g->vertex_size, sizeof(vertex_t *), _ind_cmp);
            for(i = 0; i < web_g->vertex_size && i < IND_TOP; i++) {
                fprintf(fp, "%s %d\n", vertex_ptr[i]->index.addr->url.buffer, vertex_ptr[i]->ind);
            }
            qsort(vertex_ptr, web_g->vertex_size, sizeof(vertex_t *), _pr_cmp);
            for(i = 0; i < web_g->vertex_size && i < PR_TOP; i++) {
                fprintf(fp, "%s %lf\n", vertex_ptr[i]->index.addr->url.buffer, vertex_ptr[i]->pr);
            }
            
            free(vertex_ptr);
        }
        fclose(fp);
    } else {
	fprintf(stderr, "Can't open file 'top10.dat'.\n");
    }
}

void calc_degree(webg_t *web_g)
{
    int i;
    for(i = 0; i < web_g->vertex_size; i++) {
	web_g->vertex[i].ind = 0;
	web_g->vertex[i].outd = 0;
    }
    for(i = 0; i < web_g->vertex_size; i++) {
	elem_t *iter;
	vertex_info_t *vertex_info;
	/* edge vid and url */
	for(iter = list_first(&web_g->vertex[i].edge); iter != list_end(&web_g->vertex[i].edge); iter = list_next(iter)) {
	    vertex_info = *(vertex_info_t **)iter->buffer;
	    if(vertex_info->vid != -1) {
		web_g->vertex[vertex_info->vid].ind++;
		web_g->vertex[i].outd++;
	    }
	}
    }   
}

void gen_ind_cdf(webg_t *web_g)
{
    FILE *ind_fp = fopen("indcdf.dat", "w");
    if(ind_fp != NULL) {
        int *ind_vector;
        if((ind_vector = (int *) malloc(sizeof(int) * web_g->vertex_size + 1)) != NULL) {
            memset(ind_vector, 0, sizeof(int) * web_g->vertex_size + 1);
            int i;
            for(i = 0; i < web_g->vertex_size; i++) {
                ind_vector[web_g->vertex[i].ind]++;
            }
            for(i = 1; i < web_g->vertex_size + 1; i++) {
                ind_vector[i] += ind_vector[i - 1];
                fprintf(ind_fp, "%d %lf\n", i, (double)ind_vector[i] / web_g->vertex_size);
                if(web_g->vertex_size == ind_vector[i]) break;
            }
            free(ind_vector);
        }
        fclose(ind_fp);
    } else {
	fprintf(stderr, "Can't open file 'indcdf.dat'\n");
    }
}


void gen_ind_pagerank(webg_t *web_g, char *path)
{
    FILE *rfp = fopen(path, "r");
    if(rfp != NULL) {
        FILE *wfp = fopen("checkresult.dat", "w");
	if(wfp != NULL) {
            char url[4096];
	    vertex_t *vtex;
            while(!feof(rfp)) {
                bzero(url, 4096);
                fgets(url, 4096, rfp);
		strtok(url, "\n");
	        vtex = find_vertex(web_g, url);
		if(vtex != NULL) {
		    fprintf(wfp, "%s %d %lf\n", vtex->index.addr->url.buffer, vtex->ind, vtex->pr);
		} else {
		    fprintf(wfp, "%s - -\n", url);
		}
            }
	    fclose(wfp);
	} else {
	    fprintf(stderr, "Can't open file 'checkresult.dat'.\n");
	}
        fclose(rfp);
    } else {
	fprintf(stderr, "Can't open file '%s'.\n", path);
    }
}

void calc_pagerank(webg_t *web_g)
{
    calc_degree(web_g);
    double *pr_vector;
    int i, flag = web_g->vertex_size;
    double sum;
    if((pr_vector = (double *) malloc(sizeof(double) * web_g->vertex_size)) != NULL) {
	for(i = 0; i < web_g->vertex_size; i++) {
	    pr_vector[i] = 1.0;
	}
	while(flag) {
            for(i = 0; i < web_g->vertex_size; i++) {
                elem_t *iter;
                vertex_info_t *vertex_info;
                /* edge vid and url */
                for(iter = list_first(&web_g->vertex[i].edge); iter != list_end(&web_g->vertex[i].edge); iter = list_next(iter)) {
                    vertex_info = *(vertex_info_t **)iter->buffer;
	    	    if(vertex_info->vid != -1) {
	    	        web_g->vertex[vertex_info->vid].pr += (1.0 / web_g->vertex[i].outd) * pr_vector[i];
	    	    }
		}
	    }
	    
	    flag = web_g->vertex_size;
	    for(i = 0, sum = 0; i < web_g->vertex_size; i++) {
		/*
		web_g->vertex[i].pr = web_g->vertex[i].pr * (1.0 - DAMPING_FACTOR) + DAMPING_FACTOR / web_g->vertex_size;
		*/
		sum += web_g->vertex[i].pr;
	    }
	    for(i = 0; i < web_g->vertex_size; i++) {
		web_g->vertex[i].pr /= sum;
		if(fabs(web_g->vertex[i].pr - pr_vector[i]) <= PAGE_RANK_PRATE) {
		    flag--;
		}
		pr_vector[i] = web_g->vertex[i].pr;
	    }
	    for(i = 0; i< web_g->vertex_size && flag; i++) {
		web_g->vertex[i].pr = 0.0;
	    }
	}
	free(pr_vector);
    }
    
}

void gen_graphviz(webg_t *web_g)
{
    FILE *webg_fp = fopen("sitemap.dot", "w");
    if(webg_fp != NULL) {
        int i;
        fprintf(webg_fp, "digraph webgraph{\n");
        for(i = 0; i < web_g->vertex_size; i++) {
            fprintf(webg_fp, "%d;\n", web_g->vertex[i].index.vid); /* vid */
        }
        fprintf(webg_fp, "\n");
        for(i = 0; i < web_g->vertex_size; i++) {
            elem_t *iter;
            vertex_info_t *vertex_info;
            /* edge vid and url */
            for(iter = list_first(&web_g->vertex[i].edge); iter != list_end(&web_g->vertex[i].edge); iter = list_next(iter)) {
                vertex_info = *(vertex_info_t **)iter->buffer;
		if(web_g->vertex[i].index.vid != -1) {
                    fprintf(webg_fp, "%d -> %d;\n", web_g->vertex[i].index.vid, vertex_info->vid);
		}
            }
            
        }
        fprintf(webg_fp, "}\n");
        fclose(webg_fp);
    } else {
	fprintf(stderr, "Can't open file 'sitemap.dot'.\n");
    }

}

void destroy_webg(webg_t *web_g)
{
    int i;
    for(i = 0; i < web_g->vertex_size; i++) {
        list_destroy(&web_g->vertex[i].edge);
    }
    free(web_g->vertex);
    dict_destroy(&web_g->vertex_dict);
}

void printWebg(webg_t *web_g)
{
    FILE *webg_fp = fopen("webgraph.dat", "w");
    if(webg_fp != NULL) {
        int i;
        fprintf(webg_fp, "%d\n", web_g->vertex_size);
        for(i = 0; i < web_g->vertex_size; i++) {
            fprintf(webg_fp, "%d\n", web_g->vertex[i].index.vid); /* vid */
            fprintf(webg_fp, "%s\n", web_g->vertex[i].index.addr->url.buffer); /* url */
            fprintf(webg_fp, "%d\n", list_size(&web_g->vertex[i].edge)); /* edge size */
            elem_t *iter;
            vertex_info_t *vertex_info;
            /* edge vid and url */
            for(iter = list_first(&web_g->vertex[i].edge); iter != list_end(&web_g->vertex[i].edge); iter = list_next(iter)) {
                vertex_info = *(vertex_info_t **)iter->buffer;
                fprintf(webg_fp, "%d %s\n", vertex_info->vid, vertex_info->url.buffer);
            }
        }
        fclose(webg_fp);
    } else {
	fprintf(stderr, "Can't open file 'webgraph.dat'.\n");
    }
    FILE *dict_fp = fopen("link.dat", "w");
    if(dict_fp != NULL) {
        int ix;
        for(ix = 0; ix < web_g->vertex_dict.size; ix++) {
            bucket_elem_t *elem = web_g->vertex_dict.table[ix].next;
            while(elem != NULL) {
                vertex_info_t *v = elem->value;
            	/*
                printf("vertex[%d].vid: %d\n", v->vid, v->vid);
                printf("vertex[%d].len: %d\n", v->vid, v->url.len);
                printf("vertex[%d].url: %s\n", v->vid, v->url.buffer);
            	*/
                fprintf(dict_fp, "%s\n", v->url.buffer);
                elem = elem->next;
            }
        }
        fclose(dict_fp);
    } else {
	fprintf(stderr, "Can't open file 'link.dat'.\n");
    }
}
