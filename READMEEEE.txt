


****To do List:

1) Linked List ***********
    -> Make LList.h***********
    -> Implement functions of LList.h in LList.c***********
    
2) Graphs
    -> Make Graph.h********
    -> Implement functions of Graph.h in Graph.c************
    
3) Read Data file
    -> Make ReadData.h***************
    -> GetCollection()****************
    -> GetGraph(List_of_urls)
    -> GetInvertedList(List_of_urls)
    -> Implement functions of ReadData.h in ReadData.c
 

4) Pagerank.c
    -> Calls ReadData.h to read web pages and create the graph
    -> performs the pagerankfunction
    -> outputs result to pagerank.txt
    
    
5) Binary Search Tree
    -> Make BST.h
    -> Implement functions of BST.h in BST.c
    
6) Invertedindex.c
    -> calls ReadData and gets a list from GetCollection()
    -> InvertedIndex InvertedIdx <- Getinvertedlist(lsit of urls)
    -> output InvertedIdx to "invertedindex.txt"
    
7)  searchpagerank.c
    -> matched_Url_list <- findMatchedUrls("invertedIndex.txt",	queryWords)
    -> matched_Urls_with_PR <- findPagerank("pagerankList.txt",	matched_Url_list)
    -> Output	ordered	urls on	stdout
    
8)  Compile an executable for testing
    -> should make h files for all main function for testing?
    -> test against ex1 ex2 ex3
