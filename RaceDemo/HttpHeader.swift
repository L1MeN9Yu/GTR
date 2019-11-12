//
// Created by Mengyu Li on 2019-08-08.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public struct HttpHeader: Codable {
    //"Access-Control-Allow-Credentials: true"
    let accessControlAllowCredentials: String?
    //"Access-Control-Allow-Origin: *"
    let accessControlAllowOrigin: String?
    //"Content-Encoding: gzip"
    let contentEncoding: String?
    //"Content-Type: application/json"
    let contentType: String?
    //"Date: Thu, 08 Aug 2019 07:40:53 GMT"
    let date: String?
    //"Last-Modified: Thu, 08 Aug 2019 07:40:53 GMT"
    let lastModified: String?
    //"Referrer-Policy: no-referrer-when-downgrade"
    let referrerPolicy: String?
    //"Server: nginx"
    let server: String?

    enum CodingKeys: String, CodingKey {
        case accessControlAllowCredentials = "Access-Control-Allow-Credentials"
        case accessControlAllowOrigin = "Access-Control-Allow-Origin"
        case contentEncoding = "Content-Encoding"
        case contentType = "Content-Type"
        case date = "Date"
        case lastModified = "Last-Modified"
        case referrerPolicy = "Referrer-Policy"
        case server = "Server"
    }
}
