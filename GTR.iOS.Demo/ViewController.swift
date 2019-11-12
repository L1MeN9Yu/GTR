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
        self.get()
        //        self.put()
        //        self.brotli()
        //        self.download()
//        for _ in 0...10 {
//                        URLSessionDemo.fetch()
//            self.brotli()
//        }
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
        GTR.setup(agent: type(of: self), logger: type(of: self), configuration: type(of: self))
    }

    private func get() {
        GetDemo().race { destination in
            switch destination {
            case .success(let goal):
                print("\(goal)")
                break
            case .failure(_):
                break
            }
        }
    }

    private func put() {

    }

    private func brotli() {
        BrotliDemo().race { destination in
            switch destination {
            case .success(let goal):
                print("\(goal)")
            case .failure(_):
                break
            }
        }
    }

    private func download() {
        //ToDo [L1MeN9Yu]
//        DownloadDemo.request(progress: { now, total in
//            print("now = \(now) || total = \(total)")
//        }, complete: { destination in
//            switch destination {
//            case .win(let responseData):
//                return
//            case .failure(_):
//                return
//            }
//        })
    }
}

extension ViewController: Driver {
    public static var identity: [String: Encodable]? {
        [
            "Author": "L1MeN9Yu",
            "email": "baal998.mophisto@gmail.com",
        ]
    }
    public static var userAgent: String? {
        nil
    }
}

extension ViewController: Horn {
    public class func whistle(type: HornType, message: String, filename: String, function: String, line: Int) {
        print("[\(type.prefix)] => \(message)")
    }
}

extension ViewController: GearBox {

}