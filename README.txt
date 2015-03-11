******************COP-290 (DESIGN PRACTICES)**********************
***Authors:Aayush Gautam , Akash Tanwar , Vivek Kumar          ***
***DESCRIPTION:Pitara-Personal LAN File Storage 	           ***
***Date:4-02-2015                                              ***
***TIME:15:50                                                  ***
******************************************************************


******Instructions************************************************
Starting the app:
		For spawning server: Open the main folder in terminal.
							 Type 'make server' for making a server with port number 5005 by default
							 Type 'make server port=xyz' for making a server with port number xyz
		For spawning the client: Open the main folder in terminal.
								 Type 'make client' for making a client with host as 'localhost' and port number '5005' by default
								 Type 'make client server=ip port' for making a server with host as 'ip' and port number 'port'

Using the Client GUI:
					Login_Screen:
									if you already have a login username and password you can directly login and jump to main control window.
									if you have forgotten your password then you can click on forgot password and follow the procedures to retreive your password
									Or you may choose to sign up. It's completely free of cost. :)
					Sign Up screen:
									To sign up , simply give us your name, choose a username, enter the password, answer the retreive password question and you are good to go. Alw
					Forgot Password screen:
									In case you forget your password in future then you may retreive it by entering your username and the answer of security question that you answered during sign up.
					The main control screen:
									View data on Pitara: This button will show your data on Pitara in the tree view on the screen. Note that this is different from your data on your folder since 						when you delete your data on your folder it does not get deleted on Pitara for data safety purposes.
									Upload: This lets you upload any file manually of your choice. This way you don't have to keep that file on your laptop anymore.
									Download: First click on 'view data on pitara' then select any file of your choice. Click download and that file will be downloaded to your MyDownloads folder in the ./Src/Client/clientfolder/MyDownloads directory. 
									Share: You can select a file, enter the username in the text field above share, and click share. Boom! The file gets shared if the username is valid.
									Sync: Just keep your files in MyPitara folder and that shall be kept in sync with the server whenever you press sync. All the modified files will be sent to the server as next versions, all the added files will be added to the server as version 1 and all the deleted files will not be deleted on the server. You can manually delete them at any time you want to. and you can download them at any time you want.
									Delete: Just select a file and click delete. Boom! That file will be deleted from the server. Be careful, all the versions of that file will be deleted.
									Quit: This will quit the app and you can open it with all your data just the same as you left it.