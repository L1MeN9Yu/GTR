//
//  gtr_curl_map.c
//  GTR
//
//  Created by Mengyu Li on 2019/11/14.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

#include <stddef.h>
#include "gtr_curl_map.h"

const char *gtr_curl_map_protocol(long protocol) {
    switch (protocol) {
        case (1 << 0) :
            return "HTTP";
        case (1 << 1) :
            return "HTTPS";
        case (1 << 2) :
            return "FTP";
        case (1 << 3) :
            return "FTPS";
        case (1 << 4) :
            return "SCP";
        case (1 << 5) :
            return "SFTP";
        case (1 << 6) :
            return "TELNET";
        case (1 << 7) :
            return "LDAP";
        case (1 << 8) :
            return "LDAPS";
        case (1 << 9) :
            return "DICT";
        case (1 << 10) :
            return "FILE";
        case (1 << 11) :
            return "TFTP";
        case (1 << 12) :
            return "IMAP";
        case (1 << 13) :
            return "IMAPS";
        case (1 << 14) :
            return "POP3";
        case (1 << 15) :
            return "POP3S";
        case (1 << 16) :
            return "SMTP";
        case (1 << 17) :
            return "SMTPS";
        case (1 << 18) :
            return "RTSP";
        case (1 << 19) :
            return "RTMP";
        case (1 << 20) :
            return "RTMPT";
        case (1 << 21) :
            return "RTMPE";
        case (1 << 22) :
            return "RTMPTE";
        case (1 << 23) :
            return "RTMPS";
        case (1 << 24) :
            return "RTMPTS";
        case (1 << 25) :
            return "GOPHER";
        case (1 << 26) :
            return "SMB";
        case (1 << 27) :
            return "SMBS";
        default:
            break;
    }
    return "unknown";
}

const char *gtr_curl_map_http_version(long http_version) {
    switch (http_version) {
        case 1:
            return "HTTP_VERSION_1_0";
        case 2:
            return "HTTP_VERSION_1_1";
        case 3:
            return "HTTP_VERSION_2_0";
        case 4:
            return "HTTP_VERSION_2TLS";
        case 5:
            return "HTTP_VERSION_2_PRIOR_KNOWLEDGE";
        case 30:
            return "HTTP_VERSION_3";
        default:
            break;
    }
    return "unknown";
}
