//
// Created by Mengyu Li on 2019/11/13.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

public extension ResponseInfo {
    /**
        |--NAMELOOKUP
        |--|--CONNECT
        |--|--|--APPCONNECT
        |--|--|--|--PRETRANSFER
        |--|--|--|--|--STARTTRANSFER
        |--|--|--|--|--|--TOTAL
        |--|--|--|--|--|--REDIRECT
    **/
    struct Time: Codable {
        let nameLookup: Double
        let connect: Double
        let appConnect: Double
        let preTransfer: Double
        let startTransfer: Double
        let total: Double
        let redirect: Double

        enum CodingKeys: String, CodingKey {
            case nameLookup = "NAME_LOOKUP_TIME"
            case connect = "CONNECT_TIME"
            case appConnect = "APP_CONNECT_TIME"
            case preTransfer = "PRE_TRANSFER_TIME"
            case startTransfer = "START_TRANSFER_TIME"
            case total = "TOTAL_TIME"
            case redirect = "REDIRECT_TIME"
        }
    }
}

extension ResponseInfo.Time: CustomStringConvertible {
    public var description: String {
        """
        nameLookup : \(nameLookup)
        connect : \(connect)
        appConnect : \(appConnect)
        preTransfer : \(preTransfer)
        startTransfer : \(startTransfer)
        total : \(total)
        redirect : \(redirect)
        """
    }
}