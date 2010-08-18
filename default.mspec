unless ENV['HOME']
  if ENV['USERPROFILE']
    ENV['HOME'] = ENV['USERPROFILE']
  elsif ENV['HOMEDRIVE'] && ENV['HOMEPATH']
    ENV['HOME'] = ENV['HOMEDRIVE'] + ENV['HOMEPATH']
  end
end

class MSpecScript
  set :flags, %w'-I ext -r date'
  set :files, ['spec/date/*', 'spec/datetime/*']
end
