//
// Created by Mengyu Li on 2018-12-06.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation
import GTR

struct RaceDemo: Race {
    let language: String
    let pageNo: Int = 1
    let pageSize: Int = 20

    var url: String {
//        return "https://cichang.hjapi.com/v3/user/me/book/12254/share"
        return "http://dict.hjapi.com/v10/read/news/listByPage"
    }

    var timeout: UInt32 {
        return 10
    }

    var method: GTR.Method {
        return .get
    }

    var parameters: [String: Encodable]? {
        return ["language": self.language, "pageNo": self.pageNo, "pageSize": self.pageSize]
    }
}

extension RaceDemo {
    @discardableResult
    static func fetch(complete: @escaping GTR.Result) -> UInt32 {
        let race = RaceDemo(language: "en")
        return GTR.race(race: race, complete: complete)
    }
}

struct CompleteLearning: Race {
    let trainingCampID: UInt
    let lessonId: UInt
    let learningSeconds: TimeInterval

    var url: String {
        return "https://trainingcamp.hjapi.com" + "/v1/training/\(trainingCampID)/lesson/finished"
    }

    var method: GTR.Method {
        return .put
    }

    var parameters: [String: Encodable]? {
        return [
            "lessonId": self.lessonId,
            "studySeconds": self.learningSeconds
        ]
    }

    @discardableResult
    static func put(courseID: UInt, lessonId: UInt, learningSeconds: TimeInterval, complete: @escaping GTR.Result) -> UInt32 {
        let request = self.init(trainingCampID: courseID, lessonId: lessonId, learningSeconds: learningSeconds)
        return GTR.race(race: request, complete: complete)
    }
}
