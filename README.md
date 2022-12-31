# Mirror

Mirror is an experimental header-only unit test declaration library and associated tooling. It's currently used for the unit tests in my libraries, in particular [Ghost](https://github.com/ludocode/ghost) and soon [MPack](https://github.com/ludocode/mpack) and [Pottery](https://github.com/ludocode/pottery).

It's a unit test declaration *library*, not a *framework*, because it's designed to have an interchangeable backend to use with any runner. This isn't working yet. Mirror provides its own primitive runner and currently this is the only runner that works.

The eventual goal of this (besides just being a nicer way to declare unit tests) is to allow projects to include the unit tests of their source-only dependencies within their own unit test suites. This tests the libraries under the project's configuration of them and within the project's own unit test framework. I am nowhere near achieving this goal, but you can sort of see the bits and pieces here if you squint hard enough.

This is all unfinished, undocumented and highly experimental. Don't use it yet (unless you are contributing to one of the libraries linked above.)
