//
// Created by Mengyu Li on 2018-12-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

public struct RaceError: Error {
    let responseInfo: ResponseInfo?, errorCode: Int32, errorMessage: String
}

extension RaceError: CustomStringConvertible {
    public var description: String {
        """
        response info : \((responseInfo?.description ?? ""))
        error code : \(errorCode)
        message : \(errorMessage)
        """
    }
}
