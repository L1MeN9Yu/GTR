//
// Created by Mengyu Li on 2018-12-25.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation
import GTR

//private let DownloadFileURL = "https://pm.myapp.com/invc/xfspeed/qqpcmgr/module_update/Lemon_1.0.6.dmg"
//private let FilePath = NSTemporaryDirectory() + "Lemon_1.0.6.dmg"
//
//struct DownloadDemo: LongDistanceRace {
//    var url: String { return DownloadFileURL }
//    var method: GTR.Method { return .download }
//    var filePath: String { return FilePath }
//    let progress: ((UInt64, UInt64) -> Void)?
//
//    @discardableResult
//    static func request(progress: ((UInt64, UInt64) -> Void)?, complete: @escaping GTR.Result) -> UInt32 {
//        let race = self.init(progress: progress)
//        return GTR.longDistanceRace(race: race, complete: complete)
//    }
//}
