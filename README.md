# Email-Account-C-
Email account implementation using C++

First a custom templated "vector" class is made from scratch.

Class ISP contains all possible email accounts. Messages can be sent between accounts on the same ISP.
Each account has functionality for three folders: Drafts, Inbox and Outbox.

Messages in Drafts are read/write and support a "send" function.
Messages in Inbox and Outbox are read-only. Inbox and Outbox support "reply" and "forward" functions.
New messages can be created in Drafts only.
Messages in any folder can be deleted.
All folders support a "display" function to print the list of messages in the folder.
Messages support a "print" function to print the contents of the message.

![ISP Structure](https://github.com/ssalim5/Email-Account-C-/blob/master/ISP.JPG)
![Account Structure](https://github.com/ssalim5/Email-Account-C-/blob/master/Email%20Account.JPG)
