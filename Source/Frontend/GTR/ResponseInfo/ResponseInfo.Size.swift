//
// Created by Mengyu Li on 2019/11/13.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public extension ResponseInfo {
    struct Size: Codable {
        public let upload: Double
        public let download: Double
        public let header: Double
        public let request: Double

        enum CodingKeys: String, CodingKey {
            case upload = "UPLOAD_SIZE"
            case download = "DOWNLOAD_SIZE"
            case header = "HEADER_SIZE"
            case request = "REQUEST_SIZE"
        }
    }
}

extension ResponseInfo.Size: CustomStringConvertible {
    public var description: String {
        """
        upload : \(upload)
        download : \(download)
        header : \(header)
        request : \(request)
        """
    }
}