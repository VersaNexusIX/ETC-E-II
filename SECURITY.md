Thank you for using **ETC-E-II**! We take the security of this project seriously, even as an experimental tool. Below you will find our security policies and guidelines for reporting vulnerabilities.

## ❈ Security Status: Experimental
> [!CAUTION]
> **ETC-E-II** is a personal, educational hobby project created by **VersaNexusIX**. 
> - The cryptographic primitives (Sponge Permutation) are **custom-made** and have **not** been audited by professional cryptographers.
> - This software is intended for learning and securing non-critical data. 
> - **Do not** use this for high-stakes production environments or to protect extremely sensitive data.

## ❈ Supported Versions
We only provide security updates for the most recent version of the software.

| Version | Supported          |
| ------- | ------------------ |
| 2.2.x   | ✅ Supported       |
| < 2.2   | ❌ No Longer Supported |

## ❈ Reporting a Vulnerability
If you discover a security flaw or a potential exploit in the encryption engine or the file handling logic, please follow these steps:

1.  **Open an Issue**: You can open a "Security Issue" on the [GitHub Repository](https://github.com/VersaNexusIX).
2.  **Provide Details**: Describe the vulnerability, how it can be triggered, and its potential impact (e.g., "Key leakage," "Tag collision," or "Buffer overflow").
3.  **Proof of Concept**: If possible, include a minimal script or steps to reproduce the issue.

We will do our best to acknowledge your report within 48–72 hours and provide a fix as soon as possible.

## ❈ Disclosure Policy
We believe in responsible disclosure. We ask that you give us a reasonable amount of time to fix the issue before sharing details publicly to protect other users of the software.

## ❈ Best Practices for Users
To keep your data as safe as possible while using ETC-E-II:
* **Strong Passwords**: The KDF is hardened, but a weak password is still vulnerable to dictionary attacks.
* **Keep Your Keys Safe**: Remember that once a file is sealed, there is no "password reset" or recovery backdoor.
* **Verify Tags**: Always check if the system reports a "DONE" status. If the authentication tag fails, do not trust the decrypted output.
