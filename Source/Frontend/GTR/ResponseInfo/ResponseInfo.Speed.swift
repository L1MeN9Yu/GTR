//
// Created by Mengyu Li on 2019/11/13.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public extension ResponseInfo {
    struct Speed: Codable {
        public let upload: CLong
        public let download: CLong

        enum CodingKeys: String, CodingKey {
            case upload = "UPLOAD_SPEED"
            case download = "DOWNLOAD_SPEED"
        }
    }
}

extension ResponseInfo.Speed: CustomStringConvertible {
    public var description: String {
        """
        upload : \(upload)
        download : \(download)
        """
    }
}