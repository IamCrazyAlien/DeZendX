/*
* Copyright (c) 1997 and 1998 Netscape Communications Corporation
* (http://home.netscape.com/misc/trademarks.html)
*/


#ifndef MIME_H
#define MIME_H

#include "nsmail.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIME_OK					0
#define MIME_ERR_UNINITIALIZED                  -1	/* don't modify */
#define MIME_ERR_INVALIDPARAM                   -2
#define MIME_ERR_OUTOFMEMORY                    -3
#define MIME_ERR_UNEXPECTED                     -4
#define MIME_ERR_IO                             -5
#define MIME_ERR_IO_READ                        -6
#define MIME_ERR_IO_WRITE                       -7
#define MIME_ERR_IO_SOCKET                      -8
#define MIME_ERR_IO_SELECT                      -9
#define MIME_ERR_IO_CONNECT                     -10
#define MIME_ERR_IO_CLOSE                       -11
#define MIME_ERR_PARSE                          -12
#define MIME_ERR_TIMEOUT                  	-13
#define MIME_ERR_INVALID_INDEX                  -14
#define MIME_ERR_CANTOPENFILE                   -15
#define MIME_ERR_CANT_SET                       -16
#define MIME_ERR_ALREADY_SET                    -17
#define MIME_ERR_CANT_DELETE                    -18
#define MIME_ERR_CANT_ADD                       -19
/* reserved by nsmail.h codes -20 through -30 */
	
#define MIME_ERR_EOF                            -82
#define MIME_ERR_EMPTY_DATASINK                 -83
#define MIME_ERR_ENCODE                         -84
#define MIME_ERR_NO_SUCH_HEADER                 -85
#define MIME_ERR_NO_HEADERS                     -86
#define MIME_ERR_NOT_SET                        -87
#define MIME_ERR_NO_BODY                        -88
#define MIME_ERR_NOT_FOUND                      -89
#define MIME_ERR_NO_CONTENT_SUBTYPE             -90
#define MIME_ERR_INVALID_ENCODING               -91
#define MIME_ERR_INVALID_BASICPART              -92
#define MIME_ERR_INVALID_MULTIPART              -93
#define MIME_ERR_INVALID_MESSAGEPART    	-94
#define MIME_ERR_INVALID_MESSAGE		-95
#define MIME_ERR_INVALID_CONTENTTYPE	    	-96
#define MIME_ERR_INVALID_CONTENTID              -97
#define MIME_ERR_NO_DATA                        -98
#define MIME_ERR_NOTIMPL		        -99
#define MIME_ERR_EMPTY_BODY	                -100
#define MIME_ERR_EMPTY_BODY	                -100
#define MIME_ERR_UNSUPPORTED_PARTIAL_SUBTYPE	-101
#define MIME_ERR_MAX_NESTED_PARTS_REACHED		-102

#define MIME_BUFSIZE    1024

struct mimeDataSink;

/* ----------------------------- */
typedef enum mime_encoding_type 
{
    MIME_ENCODING_UNINITIALIZED = -1,	/* don't modify */
    MIME_ENCODING_BASE64 = 1,    
    MIME_ENCODING_QP,
    MIME_ENCODING_7BIT,  /* 7 bit  data with NO transfer encoding */
    MIME_ENCODING_8BIT,  /* 8 bit  data with NO transfer encoding */
    MIME_ENCODING_BINARY /* Binary data with NO transfer encoding */

} mime_encoding_type;


typedef enum mime_disp_type   
{
    MIME_DISPOSITION_UNINITIALIZED = -1,   	/* don't modify */
    MIME_DISPOSITION_INLINE = 1,   
    MIME_DISPOSITION_ATTACHMENT

} mime_disp_type;


typedef enum mime_content_type 
{
    MIME_CONTENT_UNINITIALIZED = -1,	/* don't modify */
    MIME_CONTENT_TEXT,
    MIME_CONTENT_AUDIO,
    MIME_CONTENT_IMAGE, 
    MIME_CONTENT_VIDEO,			   
    MIME_CONTENT_APPLICATION,
    MIME_CONTENT_MULTIPART,
    MIME_CONTENT_MESSAGEPART 

} mime_content_type;



typedef struct mime_header 
{
   char *name;
   char *value;
   struct mime_header *next;

} mime_header_t;



/* Common structure for leaf parts: Text, Audio, Video, Image and Application */
typedef struct mime_basicPart   
{
	char * content_description;	
    	mime_disp_type content_disposition;
    	char * content_disp_params;		/* content disposition parameters */
    	mime_content_type content_type;		
    	char * content_subtype;			
    	char * content_type_params;	
    	char * contentID;		
	mime_header_t * extra_headers;		/* additional X- and Content- headers */
    	char * contentMD5;			/* MD5 CRC code */
	mime_encoding_type encoding_type;	
    	void *pInternal;			/* Stuff that client can not directly manipulate */

} mime_basicPart_t; 



typedef struct mime_messagePart   
{
	char * content_description;
    	mime_disp_type content_disposition;	
    	char * content_disp_params;		/* content disposition parameters */
	mime_content_type content_type;		
    	char * content_subtype;			
    	char * content_type_params;			
    	char * contentID;				
	mime_header_t * extra_headers;		/* additional X- and Content- headers */
	mime_encoding_type encoding_type;	
	mime_header_t * extern_headers;		/* for extern-body */
	void *pInternal;			/* Stuff that client can not directly manipulate */

} mime_messagePart_t;



typedef struct mime_multiPart   
{
	char * content_description;		
    	mime_disp_type content_disposition;	
    	char * content_disp_params;		/* content disposition parameters */
	mime_content_type content_type;		
    	char * content_subtype;			
    	char * content_type_params;			
	mime_encoding_type encoding_type;	
    	char * contentID;				
	mime_header_t * extra_headers;		/* additional X- and Content- headers */
    	char * preamble;				
	void *pInternal;			/* Stuff that client can not directly manipulate */

} mime_multiPart_t; 


typedef struct mime_message
{
	mime_header_t * rfc822_headers;		/* message headers */
	void *pInternal;			/* Stuff that client can not directly manipulate */

} mime_message_t; 


typedef struct file_mime_type
{
	mime_content_type  content_type;
        char *   	   content_subtype;
        char *   	   content_params;
        mime_encoding_type mime_encoding;
/*      char *   file_extn;      */
/*      char *   file_shortname; */
} file_mime_type; 




/*************************************************************************/
/**                       BASIC (LEAF) BODY PART TYPES                  **/
/*************************************************************************/

/* Set the body-data for this part from an input stream.
 * This can only be done on a (new) mime_basicPart with empty bodydata.
 */
int mime_basicPart_setDataStream (mime_basicPart_t * in_pBasicPart,
		            	  nsmail_inputstream_t *in_pTheDataStream,
				  BOOLEAN in_fCopyData);

/* Set the body-data for this part from a buffer.
 * This can only be done on a (new) mime_basicPart with empty bodydata.
 */
int mime_basicPart_setDataBuf (mime_basicPart_t * in_pBasicPart,
			       unsigned int in_size,
			       const char * in_pDataBuf,
			       BOOLEAN in_fCopyData);

/* Deletes the bodyData for this part. */
int mime_basicPart_deleteData (mime_basicPart_t * in_pBasicPart);

/* gives the size of the body-data in bytes */
int mime_basicPart_getSize (mime_basicPart_t * in_pBasicPart,
			    unsigned int * out_pSize);

/* returns the body-data after any decoding in an input stream */
/* If in_pFileName is NULL returns a memory based input-stream */
/* If in_pFileName is not NULL, returns the specified file based */
/* input-stream to the data */
int mime_basicPart_getDataStream (mime_basicPart_t *in_ppBasicPart,
				  char * 	    in_pFileName, 
			          nsmail_inputstream_t **out_ppDataStream);

/* returns the body-data after any decoding in a buffer */
int mime_basicPart_getDataBuf (mime_basicPart_t * in_pBasicPart,
			       unsigned int * out_pSize,
                               char **out_ppDataBuf );

/* Writes out byte stream for this part with MIME headers and encoded body-data */
int mime_basicPart_putByteStream (mime_basicPart_t * in_pBasicPart,
			    	  nsmail_outputstream_t *in_pOutput_stream);

/* Frees up the basic-part and all internal structures. User should set in_pBasicPart = NULL up on return*/
int mime_basicPart_free_all (mime_basicPart_t * in_pBasicPart);


/*************************************************************************/
/**                       COMPOSITE BODY PART TYPES                     **/
/*************************************************************************/

/*=========================== MULTI PART =================================*/

/* Adds a file as basic part to the multi-part. 	      */
/* If in_pref_file_encoding  > 0 attempts to use the encoding */
/* for the file-attachment. If encoding is not valid for the  */
/* file-type overrides with the correct value.                */
int mime_multiPart_addFile (mime_multiPart_t * in_pMultiPart,
                            char *   in_pFileFullName,
			    mime_encoding_type in_pref_file_encoding,
                            int * out_pIndex_assigned);

/* Adds a basic part to the multi-part. */
int mime_multiPart_addBasicPart (mime_multiPart_t * in_pMultiPart,
		            	 mime_basicPart_t * in_pBasicPart,
				 BOOLEAN in_fClone,
		            	 int * out_pIndex_assigned);

/* Adds a message part to the multi-part. */
int mime_multiPart_addMessagePart (mime_multiPart_t * in_pMultiPart,
		            	   mime_messagePart_t * in_pMessagePart,
				   BOOLEAN in_fClone,
		            	   int * out_pIndex_assigned);

/* Adds a message part to the multi-part. */
int mime_multiPart_addMultiPart (mime_multiPart_t * in_pMultiPart,
		            	 mime_multiPart_t * in_pMultiPartToAdd,
				 BOOLEAN in_fClone,
		            	 int * out_pIndex_assigned);

/* Deletes the bodyPart at the requested index from this multi-part */
int mime_multiPart_deletePart (mime_multiPart_t * in_pMultiPart,
		               int in_index);

/* returns the count of the body parts in this multi-part */
int mime_multiPart_getPartCount (mime_multiPart_t * in_pMultiPart,
		                int * out_count);

/* returns the body part  at the specified index */
int mime_multiPart_getPart (mime_multiPart_t * in_pMultiPart,
		            int  in_index,
			    mime_content_type * out_pContentType,
			    void **out_ppTheBodyPart  /* Client can cast based on pContentType */ 
			    );

/* Writes out byte stream for this part with MIME headers and encoded body-data */
int mime_multiPart_putByteStream (mime_multiPart_t * in_pMultiPart,
			    	  nsmail_outputstream_t *in_pOutput_stream);

/* Frees up the multi-part and all internal structures. User should set in_pMultiPart = NULL up on return*/
int mime_multiPart_free_all (mime_multiPart_t * in_pMultiPart);


/*=========================== MESSAGE PART =================================*/

/* creates a message part from a message structure */
int mime_messagePart_fromMessage (mime_message_t  * in_pMessage, 
                                  mime_messagePart_t ** out_ppMessagePart);

/* retrieves the message from the message part */
int mime_messagePart_getMessage (mime_messagePart_t * in_pMessagePart,
		                 mime_message_t ** out_ppMessage);

/* deletes the mime message that is the body of this part */
int mime_messagePart_deleteMessage (mime_messagePart_t * in_pMessagePart);

/* Sets the passed message as body of this message-part. It is error to set message
   when it is already set.  */
int mime_messagePart_setMessage (mime_messagePart_t * in_pMessagePart,
		                 mime_message_t * in_pMessage);

/* Writes out byte stream for this part to the output stream */
int mime_messagePart_putByteStream (mime_messagePart_t * in_pMessagePart,
			    	    nsmail_outputstream_t *in_pOutput_stream);

/* Frees up the message-part and all internal structures. User should set in_pMessagePart = NULL up on return*/
int mime_messagePart_free_all (mime_messagePart_t * in_pMessagePart);

/*============================== MESSAGE ===================================*/

/* Creates a message given text-data and a file to attach.    */
/* The file-name supplied must be fully-qualified file-name.  */
/* If either in_pText or in_pFileFullName are NULL creates a  */
/* MIME message with the non-NULL one. It is an error to pass */
/* NULL for both text and file-name parameters. 	      */
/* If in_pref_file_encoding  > 0 attempts to use the encoding */
/* for the file-attachment. If encoding is not valid for the  */ 
/* file-type overrides with the correct value.		      */
/* Returns MIME_OK on success and an error code otherwise.    */
/*                                                            */
/* Copies the data in in_pText. User responsible for freeing  */
/* in_pText up on return as needed. in_pFileFullName can also */
/* be free'd up on return.				      */
int mime_message_create (char * 	   in_pText,
			 char *		   in_pFileFullName,
			 mime_encoding_type in_pref_file_encoding,
                         mime_message_t ** out_ppMessage);

/* Adds a basic part as the body of the message. */
/* Error if Body is already present.             */
/* Sets content-type accordingly.                */
int mime_message_addBasicPart (mime_message_t * in_pMessage,
		               mime_basicPart_t * in_pBasicPart,
			       BOOLEAN in_fClone);

/* Adds the message part as the body of the message. */
/* Error if Body is already present.             */
/* Sets content-type accordingly.                    */
int mime_message_addMessagePart (mime_message_t * in_pMessage,
		            	 mime_messagePart_t * in_pMessagePart,
			         BOOLEAN in_fClone);

/* Adds the multipart as the body of the message */
/* Error if Body is already present.             */
/* Sets content-type accordingly.                */
int mime_message_addMultiPart (mime_message_t   * in_pMessage,
		               mime_multiPart_t * in_pMultiPart,
			       BOOLEAN in_fClone);

/* Deletes the bodyPart that constitutes the body of this message */
int mime_message_deleteBody (mime_message_t * pMessage);

/* returns the content_type of this message */
int mime_message_getContentType (mime_message_t * in_pMessage,
			         mime_content_type * out_content_type);

/* returns the content_subtype of this message */
int mime_message_getContentSubType (mime_message_t * in_pMessage,
			            char ** out_ppSubType);

/* returns the content_type params of this message */
int mime_message_getContentTypeParams (mime_message_t * in_pMessage,
			               char ** out_ppParams);

/* gets the value of the specified header.*/
int mime_message_getHeader (mime_message_t * in_pMessage,
			    char *  in_pName,
			    char ** out_ppValue);


/* Returns the body part  at the specified index */
/* Client to free the returned body-part. See:   */
/* 	mime_basicPart_free_all()		 */
/* 	mime_multiPart_free_all() and		 */
/* 	mime_messagePart_free_all() 		 */
int mime_message_getBody (mime_message_t * pMessage,
			  mime_content_type * out_pContentType,
			  void ** out_ppTheBodyPart  /* Client to cast this based on pContentType */
			  );

/* Writes out byte stream for this part to the output stream */
int mime_message_putByteStream (mime_message_t * in_pMessage,
			    	nsmail_outputstream_t *in_pOutput_stream);

/* Frees up the message and all internal structures. User should set in_pMessage = NULL up on return*/
int mime_message_free_all (mime_message_t * in_pMessage);


/*************************************************************************/ 
/**                      UTILITY ROUTINES                                */
/*************************************************************************/ 

/* Builds and returns a mime_header given name and value */
/* Returns NULL if either name or value is a NULL        */
mime_header_t * mime_header_new (char * in_pName, char * in_pValue);

/* Frees a mime_header_t structure. User should set in_pHdr = NULL up on return */  
int mime_header_free (mime_header_t * in_pHdr);

/* Encode a header string per RFC 2047. The encoded string can be used as the 
   value of unstructured headers or in the comments section of structured headers.
   Below, encoding must be one of  'Q' or  'B' */
int mime_encodeHeaderString (char *  in_pString, 
                             char *  in_charset,          /* charset the input string is in */
                             char in_encoding,
                             char ** out_ppString);

/* If the input string is not encoded (per RFC 2047), returns the same.
    Otherwise, decodes and returns the decoded string */
int mime_decodeHeaderString (char *  in_pString, 
                             char ** out_ppString);

/* Base64 encodes the data from input_stream and writes to output_stream */
int mime_encodeBase64 (nsmail_inputstream_t * in_pInput_stream,
                       nsmail_outputstream_t * in_pOutput_stream);

/* QuotedPrintable encodes the data from input_stream and writes to output_stream */
int mime_encodeQP (nsmail_inputstream_t * in_pInput_stream,
                   nsmail_outputstream_t * in_pOutput_stream);

/* Base64 decodes the data from input_stream and writes to output_stream */
int mime_decodeBase64 (nsmail_inputstream_t * in_pInput_stream,
                       nsmail_outputstream_t * in_pOutput_stream);

/* QuotedPrintable decodes the data from input_stream and writes to output_stream */
int mime_decodeQP (nsmail_inputstream_t * in_pInput_stream,
                   nsmail_outputstream_t * in_pOutput_stream);

/* Returns file's MIME type info based on file's extension. */
/* By default returns application/octet-stream.             */
/* If filename_ext is null returns application/octet-stream.*/
int getFileMIMEType (char * in_pFilename_ext, file_mime_type * in_pFileMIMEType);

/* Allocates memory of specified size. Recommended to be used instead of 
   malloc() for memory allocated to use with mime API. Avoids heap space
   conflicts when the application links with two or more libraries that 
   support malloc / free. Internally simply invokes malloc() and hence the 
   simantics and usage are identical to malloc(). */
void * mime_malloc (unsigned int in_size);
 
/* To be used for freeing any memory allocated by the sdk.
   Example: Data buffer returned by mime_basicPart_getDataBuf().
   Another example: Memory allocated using mime_malloc(). 
   Internally simply invokes free() and hence the simantics and 
   usage are identical to free().*/
void mime_memfree (void * in_pMem);


#ifdef __cplusplus
}
#endif


#endif /* MIME_H */
