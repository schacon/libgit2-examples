#! /usr/bin/env ruby
#

require 'rubygems'
require 'rugged'

index = Rugged::Index.new("/tmp/gittalk/.git/index");
index.refresh

index.each do |entry|
  puts "File Name: " + entry.path
  puts " Blob SHA: " + entry.sha
  puts "File Size: " + entry.file_size.to_s
  puts "File Mode: " + entry.mode.to_s
  puts "    mtime: " + entry.mtime.to_i.to_s
  puts "    ctime: " + entry.ctime.to_i.to_s
  puts "    Inode: " + entry.ino.to_s
  puts "      UID: " + entry.uid.to_s
  puts "      GID: " + entry.gid.to_s
  puts
end
