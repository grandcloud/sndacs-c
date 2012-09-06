#sndacs

sndacs library provides access to [SNDA Cloud Storage](http://www.grandcloud.cn/product/ecs).

## Installation

### libs    
   
    libcurl 7.25.0   download: http://curl.haxx.se/download.html
    openssl 1.0.1c  download: http://www.openssl.org/source/
    libxml2       download: http://www.xmlsoft.org/downloads.html

### compile

    gcc  src/sdk/*.c src/example/snda_ecs_console.c -I./src/sdk  -I/usr/local/include/ -I/usr/local/ssl/include/ -I/usr/local/include/libxml2/ -L/usr/local/ssl/lib  -lcurl  -lxml2 -lssl -lcrypto -o snda_ecs_console


## Usage

### Get Service
 
     /**
     * get service
     * @param SNDAECSHandler* handler, the handler you had initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param int ssl,whether to use https
     * @param SNDAECSResult* ret,SNDAECSResult* created from snda_ecs_init_result(), if you want to reuse this pointer, MAKE SURE invoke snda_ecs_reset_result(SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_get_service(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              int ssl, 
                              SNDAECSResult* ret);


### Put Bucket

     /**
     * put bucket
     * @param SNDAECSHandler* handler, the handler you had initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* region,region of your bucket,region currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSResult* ret,SNDAECSResult* created from snda_ecs_init_result(), if you want to reuse this pointer, MAKE SURE invoke snda_ecs_reset_result(SNDAECSResult*)
     *                to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_put_bucket(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* region, 
                              int ssl, 
                              SNDAECSResult* ret);


### Get Bucket

    /**
     * Get bucket
     * @param SNDAECSHandler* handler, the handler you had
     *       initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* prefix,the prefix parameter to the
     *   	  key of the object you want to retrieve
     * @param const char* marker,the key to start with
     * @param const char* delimiter,the character your use to
     * 		  group keys
     * @param int maxkeys,the maximum number of keys returned
     *        in the response body
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
    * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *        follow any "Location: " header that the server
     *        sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *        redirections to follow. Use this option alongside
     *        followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *        snda_ecs_init_result(), if you want to reuse this
     *        pointer, MAKE SURE invoke snda_ecs_reset_result
     *        (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_get_bucket(
                              SNDAECSHandler* handler,
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname,
                              const char* prefix, 
                              const char* marker, 
                              const char* delimiter,
                              int maxkeys, 
                              const char* region, 
                              int ssl,
                              SNDAECSFollowLocation followlocation, 
                              long maxredirects,
                              SNDAECSResult* ret);


### Delete Bucket

      /**
       * Delete bucket
       * @param SNDAECSHandler* handler, the handler you had
       *       initialized by invoking snda_ecs_init_handler()
       * @param const char* accesskey,your accessKey
       * @param const char* secretkey,your secretKey
       * @param const char* bucketname,your bucketname
       * @param const char* region,region of your bucket,region
       *         currently support "huadong-1", "huabei-1"
       * @param int ssl,whether to use https
       * @param SNDAECSFollowLocation followlocation,whether to
       *         follow any "Location: " header that the server
       *         sends as part of the HTTP header
       * @param long maxredirects,the maximum amount of HTTP
       *         redirections to follow. Use this option alongside
       *         followlocation.
       * @param SNDAECSResult* ret,SNDAECSResult* created from
       *         snda_ecs_init_result(), if you want to reuse this
       *         pointer, MAKE SURE invoke snda_ecs_reset_result
       *         (SNDAECSResult*) to reset this pointer to initial status.
       * return SNDAECSErrorCode
       */
      SNDAECSErrorCode snda_ecs_delete_bucket(
                                SNDAECSHandler* handler, 
                                const char* accesskey,
                                const char* secretkey, 
                                const char* bucketname, 
                                const char* region, 
                                int ssl,
                                SNDAECSFollowLocation followlocation, 
                                long maxredirects,
                                SNDAECSResult* ret);


### Put Bucket Policy

      /**
       * Put bucket policy
       * @param SNDAECSHandler* handler, the handler you had
       *   	   initialized by invoking snda_ecs_init_handler()
       * @param const char* accesskey,your accessKey
       * @param const char* secretkey,your secretKey
       * @param const char* bucketname,your bucketname
       * @param const char* policy,your bucket policy
       * @param int ssl,whether to use https
       * @param SNDAECSResult* ret,SNDAECSResult* created from
       *         snda_ecs_init_result(), if you want to reuse this
       *         pointer, MAKE SURE invoke snda_ecs_reset_result
       *         (SNDAECSResult*) to reset this pointer to initial status.
       * return SNDAECSErrorCode
       */
      SNDAECSErrorCode snda_ecs_put_bucket_policy(
                                SNDAECSHandler* handler, 
                                const char* accesskey,
                                const char* secretkey, 
                                const char* bucketname, 
                                const char* policy, 
                                int ssl, 
                                SNDAECSResult* ret);


### Get Bucket Policy

    /**
     * Get bucket policy
     * @param SNDAECSHandler* handler, the handler you had
     *   	   initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param int ssl,whether to use https
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_get_bucket_policy(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              int ssl, 
                              SNDAECSResult* ret);


### Delete Bucket Policy

    /**
     * Delete bucket policy
     * @param SNDAECSHandler* handler, the handler you had
     *        initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param int ssl,whether to use https
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_delete_bucket_policy(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              int ssl, 
                              SNDAECSResult* ret);


### Get Bucket Location

    /**
     * Get bucket location
     * @param SNDAECSHandler* handler, the handler you had
     *        initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param int ssl,whether to use https
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_get_bucket_location(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              int ssl, 
                              SNDAECSResult* ret);

### Put Object
 
    /**
     * Put Object
     * @param SNDAECSHandler* handler, the handler you had
     *        initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,your object name
     * @param CallbackFunPtr readFun, used as CURLOPT_READDATA,usually is snda_ecs_put_object_body
     * @param void* inputstream, data stream for upload ,usually a pointer of file opened with "rb"
     * @param long contentlength,the size of the object,in bytes
     * @param const SNDAECSUserObjectMeta* userobjectmeta,used in request headers
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_put_object(
    SNDAECSHandler* handler, 
    const char* accesskey,
    const char* secretkey, 
    const char* bucketname, 
    const char* objectname,
    CallbackFunPtr readFun, 
    void* inputstream, 
    long contentlength,
    const SNDAECSUserObjectMeta* userobjectmeta, 
    const char* region, int ssl,
    SNDAECSResult* ret)


### Head Object
 
    /**
     * Head Object
     * @param SNDAECSHandler* handler, the handler you had
     *   	   initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,your object name
     * @param SNDAECSByteRange* byterange,the specified range bytes of the object.
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *         follow any "Location: " header that the server
     *         sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *         redirections to follow. Use this option alongside
     *         followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_head_object(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* objectname, 
                              SNDAECSByteRange* byterange,
                              const char* region, 
                              int ssl, 
                              SNDAECSFollowLocation followlocation, 
                              long maxredirects,
                              SNDAECSResult* ret);


### Get Object

     /**
     * Get Object
     * @param SNDAECSHandler* handler, the handler you had
     *   	   initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,your object name
     * @param SNDAECSByteRange* byterange,the specified range bytes of the object.
     * @param CallbackFunPtr writeFun,used as CURLOPT_READFUNCTION,usually is snda_ecs_write_fun();
     * @param  void* outputstream, usually a pointer of file opend with "wb";
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *         follow any "Location: " header that the server
     *         sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *         redirections to follow. Use this option alongside
     *         followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_get_object(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* objectname, 
                              SNDAECSByteRange* byterange,
                              CallbackFunPtr writeFun, 
                              void* outputstream, 
                              const char* region, int ssl,
                              SNDAECSFollowLocation followlocation, 
                              long maxredirects,
                              SNDAECSResult* ret)


### Delete object
    
    /**
     * Delete Object
     * @param SNDAECSHandler* handler, the handler you had
     *   	   initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,your object name
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *         follow any "Location: " header that the server
     *         sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *         redirections to follow. Use this option alongside
     *         followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_delete_object(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* objectname, 
                              const char* region, 
                              int ssl,
                              SNDAECSFollowLocation followlocation, 
                              long maxredirects,
                              SNDAECSResult* ret);


### Copy Object

    /**
     * @param SNDAECSHandler* handler, the handler you had
     *   	   initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* destbucketname,the name of the destination bucket
     * @param const char* destobjectname,the key of the destination object
     * @param const char* srcbucketname,the name of the source bucket
     * @param const char* srcobjectname,the key of the source object
     * @param const SNDAECSUserObjectMeta* userobjectmeta,used in request headers
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_copy_object(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* destbucketname,
                              const char* destobjectname,
                              const char *srcbucketname,
                              const char * srcobjectname,
                              const SNDAECSUserObjectMeta* userobjectmeta,
                              const char* region, int ssl,
                              SNDAECSResult* ret);


###  Initate Multipart Upload

   
    /**
     * Initiate Multipart upload
     * @param SNDAECSHandler* handler, the handler you had
     *   	   initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,your object name
     * @param const char* region,region of your bucket,region
     *         currently support "huadong-1", "huabei-1"
     * @param const SNDAECSUserObjectMeta* userobjectmeta,used
     * 		   in request headers
     * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *         follow any "Location: " header that the server
     *         sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *         redirections to follow. Use this option alongside
     *         followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_initiate_multipart_upload(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* objectname, 
                              const SNDAECSUserObjectMeta* userobjectmeta,
                              const char* region, 
                              int ssl, 
                              SNDAECSFollowLocation followlocation, 
                              long maxredirects,
                              SNDAECSResult* ret)



### List Multipart Upload

     /**
     * List_multipart_uploads
     * @param SNDAECSHandler* handler, the handler you had
     *   	  initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* prefix,the prefix parameter to the
     *   	  key of the multipart upload you want to retrieve
     * @param const char* keymarker,the key to start with
     * @param const char* uploadidmarker,the uploadid to start with
     * @param const char* delimiter,the character your use to
     * 		  group keys
     * @param int maxuploads,the maximum number of keys returned
     *        in the response body
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *        follow any "Location: " header that the server
     *        sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *        redirections to follow. Use this option alongside
     *        followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *        snda_ecs_init_result(), if you want to reuse this
     *        pointer, MAKE SURE invoke snda_ecs_reset_result
     *        (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_list_multipart_uploads(
                              SNDAECSHandler* handler, 
                              const char* accesskey, 
                              const char* secretkey,
                              const char* bucketname, 
                              const char* prefix, 
                              const char* keymarker, 
                              const char* uploadidmarker,
                              const char* delimiter, 
                              int maxuploads, 
                              const char* region, 
                              int ssl,
                              SNDAECSFollowLocation followlocation, 
                              long maxredirects,
                              SNDAECSResult* ret)


### Abort Multipart Upload

    /**
     * Abort_multipart_uploads
     * @param SNDAECSHandler* handler, the handler you had
     *   	  initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,your object name
     * @param const char* uploadid,your uploadid for
     *  	   the multipart upload
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *        follow any "Location: " header that the server
     *        sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *        redirections to follow. Use this option alongside
     *        followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *        snda_ecs_init_result(), if you want to reuse this
     *        pointer, MAKE SURE invoke snda_ecs_reset_result
     *        (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_abort_multipart_upload(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* objectname,
                              const char* uploadid, 
                              const char* region, 
                              int ssl,
                              SNDAECSFollowLocation followlocation, 
                              long maxredirects,
                              SNDAECSResult* ret)


### Upload Part

    /**
     * Upload Part
     * @param SNDAECSHandler* handler, the handler you had
     *   	   initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,your object name
     * @param const char* uploadid,your uploadid for multipart upload
     * @param int partnumber,partnumber of this part
     * @param CallbackFunPtr readFun, used as CURLOPT_READDATA,usually is snda_ecs_put_object_body
     * @param void* inputstream, data stream for upload ,usually a pointer of file opened with "rb"
     * @param long contentlength,the size of the object,in bytes
     * @param const char* contentmd5,contentmd5 of this part(can be null)
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *         snda_ecs_init_result(), if you want to reuse this
     *         pointer, MAKE SURE invoke snda_ecs_reset_result
     *         (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_upload_part(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* objectname,
                              const char* uploadid, 
                              int partnumber, 
                              CallbackFunPtr readFun, 
                              void* inputstream,
                              long contentlength, 
                              const char* contentmd5, 
                              const char* region, 
                              int ssl,
                              SNDAECSResult* ret);


### Upload part - Copy
    
    /**
    * Upload Part - Copy 
    * @param SNDAECSHandler* handler, the handler you had
    *         initialized by invoking snda_ecs_init_handler()
    * @param const char* accesskey,your accessKey
    * @param const char* secretkey,your secretKey
    * @param const char* bucketname,the name of the destination bucket
    * @param const char* objectname,the key of the destination object
    * @param const char* uploadid,your uploadid for multipart upload
    * @param int partnumber,partnumber of this part
    * @param const SNDAECSUserObjectMeta* userobjectmeta,used in request headers
    * @param const char* srcbucketname,the name of the source bucket
    * @param const char* srcobjectname,the key of the source object
    * @param int ssl,whether to use https
    * @param SNDAECSResult* ret,SNDAECSResult* created from
    *         snda_ecs_init_result(), if you want to reuse this
    *         pointer, MAKE SURE invoke snda_ecs_reset_result
    *         (SNDAECSResult*) to reset this pointer to initial status.
    * return SNDAECSErrorCode
    */
    SNDAECSErrorCode snda_ecs_upload_part_copy(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* objectname,
                              const char* uploadid, 
                              int partnumber,
                              const char *region,
                              const SNDAECSUserObjectMeta* userobjectmeta,
                              const char* sbucket,
                              const char* sobjectname,
                              int ssl,
                              SNDAECSResult* ret)
            
            
### List Parts

    /**
     * List_Parts
     * @param SNDAECSHandler* handler, the handler you had
     *   	  initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,the object name of the
     *         multipart upload
     * @param const char* uploadid,the uploadid of the multipart
     *  	   upload
     * @param const char* partnumbermarker,the part to start with
     * @param int maxparts,the maximum number of parts returned
     *        in the response body
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *        follow any "Location: " header that the server
     *        sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *        redirections to follow. Use this option alongside
     *        followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *        snda_ecs_init_result(), if you want to reuse this
     *        pointer, MAKE SURE invoke snda_ecs_reset_result
     *        (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_list_parts(
                                SNDAECSHandler* handler, 
                                const char* accesskey, 
                                const char* secretkey,
                                const char* bucketname, 
                                const char* objectname, 
                                const char* uploadid,
                                int partnumbermarker, 
                                int maxparts, 
                                const char* region, 
                                int ssl,
                                SNDAECSFollowLocation followlocation, 
                                long maxredirects,
                                SNDAECSResult* ret)


### Complete Multipart Upload

    /**
     * Complete multipart upload
     * @param SNDAECSHandler* handler, the handler you had
     *   	  initialized by invoking snda_ecs_init_handler()
     * @param const char* accesskey,your accessKey
     * @param const char* secretkey,your secretKey
     * @param const char* bucketname,your bucketname
     * @param const char* objectname,the object name of the
     *         multipart upload
     * @param const char* uploadid,the uploadid of the multipart
     *  	   upload
     * @param const SNDAECSMultipartsMeta* partsmeta,partsmetas of
     *         the multipart upload
     * @param const char* region,region of your bucket,region
     *        currently support "huadong-1", "huabei-1"
     * @param int ssl,whether to use https
     * @param SNDAECSFollowLocation followlocation,whether to
     *        follow any "Location: " header that the server
     *        sends as part of the HTTP header
     * @param long maxredirects,the maximum amount of HTTP
     *        redirections to follow. Use this option alongside
     *        followlocation.
     * @param SNDAECSResult* ret,SNDAECSResult* created from
     *        snda_ecs_init_result(), if you want to reuse this
     *        pointer, MAKE SURE invoke snda_ecs_reset_result
     *        (SNDAECSResult*) to reset this pointer to initial status.
     * return SNDAECSErrorCode
     */
    SNDAECSErrorCode snda_ecs_complete_multipart_upload(
                              SNDAECSHandler* handler, 
                              const char* accesskey,
                              const char* secretkey, 
                              const char* bucketname, 
                              const char* objectname,
                              const char* uploadid, 
                              const SNDAECSMultipartsMeta* partsmeta, 
                              const char* region, 
                              int ssl,
                              SNDAECSFollowLocation followlocation, 
                              long maxredirects,
                              SNDAECSResult* ret)



## Copyright

Copyright (c) 2012 grandcloud.cn.
All rights reserved.