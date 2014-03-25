/*
 * Class Protocol: definitions of command and error codes
 * for the messaging protocol used in the news system project,
 * C++ Programming course.
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <map>

struct Protocol {
	enum a {
		/* Command codes, client -> server */
		COM_LIST_NG    = 1, // list newsgroups
		COM_CREATE_NG  = 2, // create newsgroup
		COM_DELETE_NG  = 3, // delete newsgroup
		COM_LIST_ART   = 4, // list articles
		COM_CREATE_ART = 5, // create article
		COM_DELETE_ART = 6, // delete article
		COM_GET_ART    = 7, // get article
		COM_END        = 8, // command end
		
		/* Answer codes, server -> client */
		ANS_LIST_NG    = 20, // answer list newsgroups
		ANS_CREATE_NG  = 21, // answer create newsgroup
		ANS_DELETE_NG  = 22, // answer delete newsgroup
		ANS_LIST_ART   = 23, // answer list articles
		ANS_CREATE_ART = 24, // answer create article
		ANS_DELETE_ART = 25, // answer delete article
		ANS_GET_ART    = 26, // answer get article
		ANS_END        = 27, // answer end
		ANS_ACK        = 28, // acknowledge
		ANS_NAK        = 29, // negative acknowledge
		
		/* Parameters */
		PAR_STRING = 40, // string
		PAR_NUM    = 41, // number
		
		/* Error codes */
		ERR_NG_ALREADY_EXISTS  = 50, // newsgroup already exists
		ERR_NG_DOES_NOT_EXIST  = 51, // newsgroup does not exist
		ERR_ART_DOES_NOT_EXIST = 52  // article does not exist
	};
	
	static std::string map(const a v){
		std::string result;
		switch (v){
		case COM_LIST_NG: result = "COM_LIST_NG"; break;
		case COM_CREATE_NG: result = "COM_CREATE_NG"; break;
		case COM_DELETE_NG: result = "COM_DELETE_NG"; break;
		case COM_LIST_ART: result = "COM_LIST_ART"; break;
		case COM_CREATE_ART: result = "COM_CREATE_ART"; break;
		case COM_DELETE_ART: result = "COM_DELETE_ART"; break;
		case COM_GET_ART: result = "COM_GET_ART"; break;
		case COM_END: result = "COM_END"; break;

		case ANS_LIST_NG: result = "ANS_LIST_NG"; break;
		case ANS_CREATE_NG: result = "ANS_CREATE_NG"; break;
		case ANS_DELETE_NG: result = "ANS_DELETE_NG"; break;
		case ANS_LIST_ART: result = "ANS_LIST_ART"; break;
		case ANS_CREATE_ART: result = "ANS_CREATE_ART"; break;
		case ANS_DELETE_ART: result = "ANS_DELETE_ART"; break;
		case ANS_GET_ART: result = "ANS_GET_ART"; break;
		case ANS_END: result = "ANS_END"; break;
		case ANS_ACK: result = "ANS_ACK"; break;
		case ANS_NAK: result = "ANS_NAK"; break;

		case PAR_STRING: result = "PAR_STRING"; break;
		case PAR_NUM: result = "PAR_NUM"; break;

		case ERR_NG_ALREADY_EXISTS: result = "ERR_NG_ALREADY_EXISTS"; break;
		case ERR_NG_DOES_NOT_EXIST: result = "ERR_NG_DOES_NOT_EXIST"; break;
		case ERR_ART_DOES_NOT_EXIST: result = "ERR_ART_DOES_NOT_EXIST"; break;
		}
		return result;
	}
};

#endif
