require File.expand_path('../../spec_helper', __FILE__)

describe "Date formatting methods" do
  it "#asctime and #ctime should use a format similar to Time" do
    Date.new(2008, 1, 2).asctime.should == 'Wed Jan  2 00:00:00 2008'
    Date.new(2008, 1, 2).ctime.should == 'Wed Jan  2 00:00:00 2008'
  end
  
  it "#to_s should use an ISO8601 format" do
    Date.new(2008, 1, 2).to_s.should == '2008-01-02'
  end

  it "#inspect should use an ISO8601 format" do
    Date.new(2008, 1, 2).inspect.should == '#<Date 2008-01-02>'
  end

  ruby_version_is "1.9" do
    it "#httpdate should use an HTTP format" do
      Date.new(2009, 1, 2).httpdate.should ==  "Fri, 02 Jan 2009 00:00:00 GMT"
    end

    it "#iso8601 should use an ISO8601 format" do
      Date.new(2009, 1, 2).iso8601.should ==  "2009-01-02"
    end

    it "#jisx03010 should use an JIS X 0301 format" do
      Date.new(2009, 1, 2).jisx0301.should ==  "H21.01.02"
      Date.new(1988, 1, 2).jisx0301.should ==  "S63.01.02"
      Date.new(1925, 1, 2).jisx0301.should ==  "T14.01.02"
      Date.new(1911, 1, 2).jisx0301.should ==  "M44.01.02"
      Date.new(1873, 1, 2).jisx0301.should ==  "M06.01.02"
      Date.new(1872, 1, 2).jisx0301.should ==  "1872-01-02"
      Date.new(1867, 1, 2).jisx0301.should ==  "1867-01-02"
    end

    it "#rfc2822 should use an RFC2822 format" do
      Date.new(2009, 1, 2).rfc2822.should ==  "Fri, 2 Jan 2009 00:00:00 +0000"
    end

    it "#rfc822 should use an RFC822 format" do
      Date.new(2009, 1, 2).rfc822.should ==  "Fri, 2 Jan 2009 00:00:00 +0000"
    end

    it "#rfc3339 should use an RFC3339 format" do
      Date.new(2009, 1, 2).rfc3339.should ==  "2009-01-02T00:00:00+00:00"
    end

    it "#xmlschema should use an ISO8601 format" do
      Date.new(2009, 1, 2).xmlschema.should ==  "2009-01-02"
    end
  end
end
