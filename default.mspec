unless ENV['HOME']
  if ENV['USERPROFILE']
    ENV['HOME'] = ENV['USERPROFILE']
  elsif ENV['HOMEDRIVE'] && ENV['HOMEPATH']
    ENV['HOME'] = ENV['HOMEDRIVE'] + ENV['HOMEPATH']
  end
end

class MSpecScript
  set :flags, %w'-I ext/date_ext -I lib -r date'
  set :files, ['spec/date/*.rb', 'spec/datetime/*.rb']
end
