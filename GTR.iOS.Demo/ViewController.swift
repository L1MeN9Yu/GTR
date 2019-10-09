//
//  ViewController.swift
//  GTR.iOS.Demo
//
//  Created by Mengyu Li on 2019/8/7.
//  Copyright © 2019 limengyu.top. All rights reserved.
//

import UIKit
import GTR

class ViewController: UIViewController {

    private lazy var actionButton = { () -> UIButton in
        let actionButton = UIButton(type: .custom)
        actionButton.setTitle("Go", for: .normal)
        actionButton.setTitleColor(.black, for: .normal)
        actionButton.addTarget(self, action: #selector(ViewController.getAction), for: .touchUpInside)
        actionButton.translatesAutoresizingMaskIntoConstraints = false
        return actionButton
    }()

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.

        self.setupUI()
        self.setupGTR()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
}

// MARK: - Actions
extension ViewController {
    @objc
    private func getAction(button: UIButton) {
        //        self.get()
        //        self.put()
        //        self.brotli()
        //        self.download()
        for _ in 0...10 {
//                        URLSessionDemo.fetch()
            self.brotli()
        }
    }
}

// MARK: - Private
extension ViewController {
    private func setupUI() {
        self.view.addSubview(self.actionButton)
        self.actionButton.leadingAnchor.constraint(equalTo: self.view.leadingAnchor).isActive = true
        self.actionButton.bottomAnchor.constraint(equalTo: self.bottomLayoutGuide.topAnchor).isActive = true
        self.actionButton.trailingAnchor.constraint(equalTo: self.view.trailingAnchor).isActive = true
        self.actionButton.heightAnchor.constraint(equalToConstant: 60).isActive = true
    }

    private func setupGTR() {
        GTR.setup(driver: type(of: self), horn: type(of: self))
    }

    private func get() {
        GetDemo.fetch() { destination in
            switch destination {
            case .win(let httpHeader, let responseData):
                if let string = String(data: responseData, encoding: .utf8) {
                    print("succeed : \(string)")
                }
                break
            case .lose(let error):
                break
            }
        }
    }

    private func put() {

    }

    private func brotli() {
        BrotliDemo.fetch() { destination in
            switch destination {
            case .win(let httpHeader, let responseData):
                if let string = String(data: responseData, encoding: .utf8) {
                    print("succeed : \(string)")
                }
            case .lose(let error):
                break
            }
        }
    }

    private func download() {
        DownloadDemo.request(progress: { now, total in
            print("now = \(now) || total = \(total)")
        }, complete: { destination in
            switch destination {
            case .win(let responseData):
                return
            case .lose(let error):
                return
            }
        })
    }
}

extension ViewController: Driver {
    public static var identity: () -> [String: Encodable]? {
        {
            [
                "Author": "L1MeN9Yu",
                "email": "baal998.mophisto@gmail.com",
            ]
        }
    }
    public static var userAgent: () -> String? {
        {
            nil
        }
    }
}

extension ViewController: Horn {
    public class func whistle(type: HornType, message: String, filename: String, function: String, line: Int) {
        print("[\(type.name)] => \(message)")
    }
}

