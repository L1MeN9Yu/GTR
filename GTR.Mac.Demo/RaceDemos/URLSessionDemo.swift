//
// Created by Mengyu Li on 2019-08-06.
// Copyright (c) 2019 limengyu.top. All rights reserved.
//

import Foundation

struct URLSessionDemo {
    static var operationQueue = { () -> OperationQueue in
        let operationQueue = OperationQueue()
        operationQueue.maxConcurrentOperationCount = 8
        return operationQueue
    }()

    static func fetch() {
        let operation = BlockOperation {
            let session = URLSession.shared
            let request = URLRequest(url: URL(string: "https://httpbin.org/brotli")!, timeoutInterval: 10)
            let task = session.dataTask(with: request) { (data: Data?, response: URLResponse?, error: Error?) -> Void in
                if let error = error { print(error.localizedDescription) }
                guard let data = data else { return }
                let string = String(data: data, encoding: .utf8) ?? ""
                print(string)
            }
            task.resume()
        }
        operationQueue.addOperation(operation)
    }
}
