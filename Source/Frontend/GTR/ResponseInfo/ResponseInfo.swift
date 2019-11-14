//
// Created by Mengyu Li on 2019/11/12.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public struct ResponseInfo: Codable {
    public let base: Base?
    public let time: Time?
    public let size: Size?
    public let speed: Speed?
    public let ssl: SSL?
    public let socket: Socket?
    public let cookie: [String]?

    enum CodingKeys: String, CodingKey {
        case base = "BASE_INFO"
        case time = "TIME_INFO"
        case size = "SIZE_INFO"
        case speed = "SPEED_INFO"
        case ssl = "SSL_INFO"
        case socket = "SOCKET_INFO"
        case cookie = "COOKIE_LIST"
    }
}

extension ResponseInfo: CustomStringConvertible {
    public var description: String {
        """
        base : 
            \(base?.description ?? "nil")
        time : 
            \(time?.description ?? "nil")
        size : 
            \(size?.description ?? "nil")
        speed :
            \(speed?.description ?? "nil")
        ssl :
            \(ssl?.description ?? "nil")
        socket :
            \(socket?.description ?? "nil")
        cookie :
            \(cookie?.description ?? "nil")
        """
    }
}