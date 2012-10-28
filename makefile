all: siteanalyzer

siteanalyzer: thread_pool.o ts_queue.o worker.o main.o thread.o  webgraph.o dict.o hash.o bloomfilter.o  thread_counter.o \
		link_parser.o link_crawler.o network.o http_url.o http_client.o http_protocol.o list.o queue.o
	gcc -o siteanalyzer -Wall -pedantic -ansi -lpthread thread_pool.o ts_queue.o worker.o main.o thread.o webgraph.o thread_counter.o\
		bloomfilter.o hash.o dict.o link_parser.o link_crawler.o network.o http_url.o http_client.o http_protocol.o list.o queue.o -g
bloomfilter.o: bloomfilter.c bloomfilter.h hash.h
	gcc -o bloomfilter.o -Wall -pedantic -ansi -c bloomfilter.c -g
webgraph_unittest: webgraph.o webgraph_unittest.o hash.o list.o dict.o
	gcc -o webgraph_unittest -Wall -pedantic -ansi webgraph.o webgraph_unittest.o hash.o list.o dict.o -g
webgraph_unittest.o: webgraph.h webgraph_unittest.c list.h 
	gcc -o webgraph_unittest.o -Wall -pedantic -ansi -c webgraph_unittest.c -g
webgraph.o: webgraph.c webgraph.h dict.h element.h hash.h list.h
	gcc -o webgraph.o -Wall -pedantic -ansi -c webgraph.c -g

main.o: ts_queue.h thread_pool.h worker.h main.c webgraph.h
	gcc -o main.o -Wall -pedantic -ansi -c main.c -g
worker.o: worker.c worker.h thread_pool.h ts_queue.h webgraph.h bloomfilter.h hash.h
	gcc -o worker.o -Wall -pedantic -ansi -c worker.c -g 

threadpool_unittest: thread_pool.o threadpool_unittest.o ts_queue.o queue.o thread.o
	gcc -o threadpool_unittest -pedantic -lpthread thread_pool.o queue.o threadpool_unittest.o ts_queue.o thread.o -g
threadpool_unittest.o: thread_pool.h threadpool_unittest.c
	gcc -o threadpool_unittest.o -Wall -pedantic -ansi -c -lpthread threadpool_unittest.c -g
thread_counter.o: thread_counter.h thread_counter.c
	gcc -o thread_counter.o -Wall -pedantic -ansi -c -lpthread thread_counter.c -g
thread.o: thread.h thread.c thread_counter.h
	gcc -o thread.o -Wall -pedantic -ansi -c -lpthread thread.c -g
thread_pool.o: thread_pool.h thread_pool.c ts_queue.h job.h thread.h thread_counter.h
	gcc -o thread_pool.o -Wall -pedantic -ansi -c -lpthread thread_pool.c -g

dict_unittest: dict_unittest.o dict.o hash.o
	gcc -o dict_unittest -Wall -pedantic dict_unittest.o dict.o hash.o -g
dict_unittest.o: dict.h hash.h dict_unittest.c
	gcc -o dict_unittest.o -Wall -pedantic -ansi -c dict_unittest.c -g
hash.o: hash.c hash.h
	gcc -o hash.o -Wall -pedantic -ansi -c hash.c -g
dict.o: dict.c dict.h
	gcc -o dict.o -Wall -pedantic -ansi -c dict.c -g

tsqueue_unittest: queue.o ts_queue.o tsqueue_unittest.o
	gcc -o tsqueue_unittest -Wall -pedantic -lpthread tsqueue_unittest.o queue.o ts_queue.o
tsqueue_unittest.o: queue.h ts_queue.h tsqueue_unittest.c
	gcc -o tsqueue_unittest.o -Wall -pedantic -ansi -c -lpthread tsqueue_unittest.c
ts_queue.o: queue.h ts_queue.h ts_queue.c
	gcc -o ts_queue.o -Wall -pedantic -ansi -c ts_queue.c
queue_unittest: queue_unittest.o queue.o
	gcc -o queue_unittest -Wall -pedantic queue_unittest.o queue.o -g
queue_unittest.o: queue_unittest.c queue.h
	gcc -o queue_unittest.o -Wall -pedantic -ansi -c queue_unittest.c -g
queue.o: queue.c queue.h element.h
	gcc -o queue.o -Wall -pedantic -ansi -c queue.c -g

list_unittest: list_unittest.o list.o
	gcc -o list_unittest -Wall -pedantic list_unittest.o list.o -g
list_unittest.o: list_unittest.c list.h
	gcc -o list_unittest.o -Wall -pedantic -ansi -c list_unittest.c -g
list.o: list.c list.h element.h
	gcc -o list.o -Wall -pedantic -ansi -c list.c -g

linkparser_unittest: linkparser_unittest.o link_parser.o list.o
	gcc -o linkparser_unittest -Wall -pedantic linkparser_unittest.o link_parser.o list.o -g
linkparser_unittest.o: linkparser_unittest.c link_parser.h 
	gcc -o linkparser_unittest.o -Wall -pedantic -ansi -c linkparser_unittest.c -g
link_parser.o: link_parser.c link_parser.h 
	gcc -o link_parser.o -Wall -pedantic -ansi -c link_parser.c -g

linkcrawler_unittest: http_client.o network.o http_protocol.o http_url.o link_crawler.o linkcrawler_unittest.o list.o link_parser.o
	gcc -o linkcrawler_unittest -Wall -pedantic http_client.o network.o http_protocol.o  \
		http_url.o link_crawler.o linkcrawler_unittest.o list.o link_parser.o -g
linkcrawler_unittest.o: link_crawler.h  linkcrawler_unittest.c
	gcc -o linkcrawler_unittest.o -Wall -pedantic -ansi -c linkcrawler_unittest.c -g
link_crawler.o: link_crawler.h link_crawler.c 
	gcc -o link_crawler.o -Wall -pedantic -ansi -c link_crawler.c -g

httpclient_unittest: http_client.o  httpclient_unittest.o network.o http_protocol.o
	gcc -o httpclient_unittest -Wall -pedantic httpclient_unittest.o http_client.o network.o http_protocol.o -g
httpclient_unittest.o: httpclient_unittest.c http_client.h
	gcc -o httpclient_unittest.o -Wall -pedantic -ansi -c httpclient_unittest.c -g
http_client.o: http_client.c http_client.h 
	gcc -o http_client.o -Wall -pedantic -ansi -c http_client.c -g
http_protocol.o: http_protocol.c http_protocol.h
	gcc -o http_protocol.o -Wall -pedantic -ansi -c http_protocol.c -g

httpurl_unittest: httpurl_unittest.o http_url.o
	gcc -o httpurl_unittest -Wall -pedantic httpurl_unittest.o http_url.o -g
httpurl_unittest.o: httpurl_unittest.c http_url.h
	gcc -o httpurl_unittest.o -Wall -pedantic -ansi -c httpurl_unittest.c -g
http_url.o: http_url.h http_url.c
	gcc -o http_url.o -Wall -pedantic -ansi -c http_url.c -g
	
network.o: network.c network.h
	gcc -o network.o -Wall -pedantic -ansi -c network.c -g


