//
// Created L1MeN9Yu on 2018/7/16.
// Copyright (c) 2018 limengyu.top. All rights reserved.
//

import Foundation

class Gearbox {
    private(set) var proxyInfo: ProxyInfo?

    private let userDefaults = UserDefaults(suiteName: "top.limengyu.GTR.GearBox")

    private let proxyKey = "GTR.GearBox.Proxy.Key"

    class var proxyInfo: Gearbox.ProxyInfo? { __gearbox.proxyInfo }
}

extension Gearbox {
    struct ProxyInfo: Codable {
        let url: String
        let port: UInt32
    }
}

// MARK: - Private
extension Gearbox {
    private func updateGTR() {
        if let proxyInfo = self.proxyInfo {
            c_gtr_proxy(true, proxyInfo.url.cString(using: .utf8), proxyInfo.port)
        } else {
            c_gtr_proxy(false, "".cString(using: .utf8), 0)
        }
    }
}

extension Gearbox {
    func start() {
        if let data = self.userDefaults?.data(forKey: self.proxyKey) {
            if let proxyInfo: ProxyInfo = try? JSONDecoder().decode(ProxyInfo.self, from: data) {
                self.proxyInfo = proxyInfo
            }
        }
        self.updateGTR()
    }

    func config(proxy enable: Bool, url: String, port: UInt32) {
        if enable {
            self.proxyInfo = ProxyInfo(url: url, port: port)
            if let proxyInfo = self.proxyInfo {
                if let data = try? JSONEncoder().encode(proxyInfo) {
                    self.userDefaults?.set(data, forKey: self.proxyKey)
                }
            }
        } else {
            self.proxyInfo = nil;
            self.userDefaults?.removeObject(forKey: self.proxyKey)
        }
        self.updateGTR()
    }
}

@_silgen_name("gtr_proxy")
func c_gtr_proxy(_ enable: Bool, _ url: UnsafePointer<Int8>?, _ port: UInt32)
