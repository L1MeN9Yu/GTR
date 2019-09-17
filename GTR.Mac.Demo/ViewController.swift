//
//  ViewController.swift
//  GTR.Mac.Demo
//
//  Created by Mengyu Li on 2019/8/7.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

import Cocoa
import GTR

class ViewController: NSViewController {

    private lazy var actionButton = { () -> NSButton in
        let actionButton = NSButton(title: "Go", target: self, action: #selector(ViewController.getAction))
        actionButton.translatesAutoresizingMaskIntoConstraints = false
        return actionButton
    }()

    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.setupUI()
        self.setupGTR()
    }

    override var representedObject: Any? {
        didSet {
            // Update the view, if already loaded.
        }
    }
}

extension ViewController {
    private func setupUI() {
        self.view.addSubview(self.actionButton)
        self.actionButton.leadingAnchor.constraint(equalTo: self.view.leadingAnchor).isActive = true
        self.actionButton.bottomAnchor.constraint(equalTo: self.view.bottomAnchor).isActive = true
        self.actionButton.trailingAnchor.constraint(equalTo: self.view.trailingAnchor).isActive = true
        self.actionButton.heightAnchor.constraint(equalToConstant: 60).isActive = true
    }

    private func setupGTR() {
        GTR.setup(driver: type(of: self), horn: type(of: self))
    }
}

extension ViewController {
    private func brotli() {
        BrotliDemo.fetch() { destination in
            switch destination {
            case .win(let header, let responseData):
                if let string = String(data: responseData, encoding: .utf8) {
                    print("succeed : \(string)")
                }
            case .lose(_, _, _):
                break
            }
        }
    }

    private func getCache() {
        GetCacheDemo.fetch { destination in
            switch destination {
            case .win(let header, let responseData):
                if let string = String(data: responseData, encoding: .utf8) {
                    print("succeed : \(string)")
                    print("\(header)")
                }
            case .lose(_, _, _):
                break
            }
        }
    }
}

extension ViewController {
    @objc
    private func getAction() {
//        self.brotli()
        self.getCache()
    }
}

extension ViewController: Driver {
    public static var identity: () -> [String: Encodable]? {
        return {
            ["Access-Token": "000347174c.db8743c66094ff2964a2b4b6791cf5db"]
        }
    }
    public static var userAgent: () -> String? {
        return {
            "GTRDemo/1.0.0"
        }
    }
}

extension ViewController: Horn {
    public class func whistle(type: HornType, message: String, filename: String, function: String, line: Int) {
        print("[\(type.name)] => \(message)")
    }
}