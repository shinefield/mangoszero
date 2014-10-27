mangos-zero server [![Build Status][build-status-img]][build-status] [![Coverity Build Status][scan-status-img]][scan-status]
==================

**mangos-zero** is an Open Source [GPL version 2 licensed](License.md) server for
running your own instance of the [World of Warcraft][wow], specifically of
[vanilla WoW][wow-1] aka. patch 1.12.

The ultimate goal of the project is to preserve the game in its' first released
version and allow players to relive the experience of the original game, since
the game is always evolving and being updated with new content.

If you are nostalgic or just missed out on the early days of World of Warcraft,
stay with us, as [vanilla WoW][wow-1] is right here.

Supporting the project
----------------------
If you like **mangos-zero** and want to support the project, there are various
options:

* [![Tip the developer!][gittip-img]][gittip] financial support: I spend roughly
  30 hours per week to work on the project, including server, scripts, database
  and documentation. A little [tip][gittip] helps pay the bills.
* testing and reporting issues: if you use **mangos-zero**, please report issues
  you find for the [server][mz-server-issues], [scripts][mz-scripts-issues],
  [database][mz-content-issues] or our [documentation][mz-docs-issues].
* submit fixes: any help is welcome! It does not matter if its' proof reading
  and fixing messages printed by the server, or if you produce documentation
  or code. As long as you have fun, be an active part of the project!

Documentation
-------------
Documentation is available at [docs.getmangos.com][mangos-zero-docs], and is
updated on regular basis with the latest changes. Documentation is available
in [source format][mz-docs], too, if you want to use it locally or contribute.

Installation
------------
To install **mangos-zero** on various target platforms, please checkout our
updated [installation documentation][mangos-zero-installation].

Contributing
------------
All information about contributing to **mangos-zero** can be found in our
[documentation for contributors][mangos-zero-contribute].

Bug Tracking
------------
**mangos-zero** uses the [BitBucket issue tracker][mz-server-issues]. If you
have found bugs, please create an issue.

License
-------
**mangos-zero** is licensed under the terms of the [GNU GPL version 2](License.md),
with a special exception allowing you to link our server against [OpenSSL][openssl].

We provide a few libraries within the `dep` directory for which different
licenses apply.

[wow]:                      http://worldofwarcraft.com/ "World of Warcraft"
[wow-1]:                    http://blizzard.com/games/wow/ "Blizzard Entertainment: World of Warcraft"

[mangos-zero]:              http://getmangos.com/ "Vanilla WoW - getmangos.com"
[mangos-zero-docs]:         http://docs.getmangos.com/ "mangos-zero documentation"
[mangos-zero-contribute]:   http://docs.getmangos.com/en/latest/contributing/ "Contributing - mangos-zero"
[mangos-zero-installation]: http://docs.getmangos.com/en/latest/installation/ "Installation - mangos-zero"

[mz-team]:                  http://bitbucket.org/mangoszero/ "mangos-zero project"
[mz-server]:                http://bitbucket.org/mangoszero/server/ "mangos-zero server repository"
[mz-scripts]:               http://bitbucket.org/mangoszero/scripts/ "mangos-zero scripts repository"
[mz-content]:               http://bitbucket.org/mangoszero/content/ "mangos-zero database repository"
[mz-docs]:                  http://bitbucket.org/mangoszero/documentation/ "mangos-zero documentation repository"

[mz-server-issues]:         http://bitbucket.org/mangoszero/server/issues "mangos-zero server issues"
[mz-scripts-issues]:        http://bitbucket.org/mangoszero/scripts/issues "mangos-zero scripts issues"
[mz-content-issues]:        http://bitbucket.org/mangoszero/content/issues "mangos-zero database issues"
[mz-docs-issues]:           http://bitbucket.org/mangoszero/documentation/issues "mangos-zero documentation issues"

[openssl]:                  http://openssl.org/ "OpenSSL: The Open Source toolkit for SSL/TLS"

[build-status]:             http://drone.io/bitbucket.org/mangoszero/server/latest "mangos-zero build status"
[build-status-img]:         http://drone.io/bitbucket.org/mangoszero/server/status.png "mangos-zero build status image"

[scan-status]:              http://scan.coverity.com/projects/365 "mangos-zero scan status"
[scan-status-img]:          http://scan.coverity.com/projects/365/badge.svg "mangos-zero scan status image"

[gittip]:                   http://gratipay.com/danielsreichenbach/
[gittip-img]:               http://img.shields.io/gratipay/danielsreichenbach.svg
